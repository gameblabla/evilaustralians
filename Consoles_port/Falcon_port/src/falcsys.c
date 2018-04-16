/*
	FalconSystem v5 - General Demo/Game System Library for the Falcon 030/060 by Orion_ [2009 - 2013]
*/

// Videl routine by Anders Eriksson from DHS
// C2P routine by Mikael Kalms from TBL, Improved by MiKRO
// IKBD routine by Patrice Mandin

#include "falcsys.h"

void	*FS_screenadrsP;
void	*FS_screenadrsL;
u16		FS_screenrez;
void	*FS_superstack;
static void	*FS_STscreen[3];
static void	*FS_TTscreen;
u8		FS_CurrentScreen;
u32		FS_Flags;
u16	FS_ScreenWidth;
u16	FS_ScreenHeight;
volatile u32  *FS_PalettePtr;
static u8	FS_ScreenBpp;

volatile u16		FS_VblAck;
volatile u16		FS_SetNewScreen;
volatile void	*FS_NewScreenAdrs;
volatile u16		FS_Vmode;

extern  void Put_Background();

volatile u16		FS_nFrames;
volatile u8	IKBD_MouseB;		// Buttons Pressed
volatile s16	IKBD_MouseX;
volatile s16	IKBD_MouseY;
volatile u8	IKBD_Joystick0;
volatile u8	IKBD_Joystick1;
volatile u8	IKBD_Keyboard[128];	// Internal
volatile void	(*FS_VBLFuncPtr)();	// Set this pointer to start a function during the VBL Interrupt (ex: sound mixer)
static unsigned long SCREEN_SIZE;
//---

char	*FalconInit(u32 flags)
{
	unsigned char res;
	FS_Flags = flags & OPT_MASK;
	FS_Vmode = (u16)(flags & VM_MASK);
	FS_PalettePtr = NULL;

	FS_ScreenWidth = 320;
	FS_ScreenHeight = 240;
	FS_ScreenBpp = 16;
	SCREEN_SIZE = ((FS_ScreenWidth * FS_ScreenHeight) * (FS_ScreenBpp / 8));
	
	// Alloc ST Screen
	if (!(FS_STscreen[0] = Mxalloc(SCREEN_SIZE, MX_STRAM)))
		return ("");

	// Double Buffer if needed
	if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
		if (!(FS_STscreen[1] = Mxalloc(SCREEN_SIZE, MX_STRAM)))
		{
			Mfree(FS_STscreen[0]);
			return ("");
		}

	// Triple Buffer if needed (allow max speed, without using FORCE_WAIT_VBL)
	if (FS_Flags & TRIPLE_BUFFER)
		if (!(FS_STscreen[2] = Mxalloc(SCREEN_SIZE, MX_STRAM)))
		{
			Mfree(FS_STscreen[1]);
			Mfree(FS_STscreen[0]);
			return ("");
		}

	// Detect CT60 cookie if CT60_MODE set !
	#ifdef _CT60
	if (FS_Flags & CT60_MODE)
	{
		if (!Supexec(GetCT60Cookie))
		{
			if (!(FS_Flags & EMULATOR_MODE))	// Allow CT60 mode without CT60 detect only if EMULATOR_MODE was set.
			{
				if (FS_Flags & TRIPLE_BUFFER)
					Mfree(FS_STscreen[2]);
				if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
					Mfree(FS_STscreen[1]);
				Mfree(FS_STscreen[0]);
				return ("");
			}
		}

		// Alloc Working Screen in Fast Ram if CT60 mode
		if (!(FS_TTscreen = Mxalloc(SCREEN_SIZE, MX_TTRAM)))
		{
			if (FS_Flags & TRIPLE_BUFFER)
				Mfree(FS_STscreen[2]);
			if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
				Mfree(FS_STscreen[1]);
			Mfree(FS_STscreen[0]);
			return ("");
		}
	}
	#endif

	// Save Current Screen & Palette
	Supexec(SaveVidel);

	// Detect if we're using VGA or RGB
	res = Supexec(SetVidel);
	if (!res)
	{
		#ifdef _CT60
		if (FS_Flags & CT60_MODE)
			Mfree(FS_TTscreen);
		#endif
		if (FS_Flags & TRIPLE_BUFFER)
			Mfree(FS_STscreen[2]);
		if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
			Mfree(FS_STscreen[1]);
		Mfree(FS_STscreen[0]);
		return ("");
	}
	// Init Low Level Interrupts
	Supexec(SetupInterrupts);
	Supexec(IKBD_Install);

	// Setup CPU Flags for Max Speed
	#ifdef _CT60
	if ((FS_Flags & CT60_MODE) && (!(FS_Flags & EMULATOR_MODE)))
	{
		Supexec(CpuSaveState);
		Supexec(CpuEnableFullCacheSuperscalar);
	}
	#endif

	// Clear Screens
	VFastClear32(FS_STscreen[0], SCREEN_SIZE, 0);
	if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
		VFastClear32(FS_STscreen[1], SCREEN_SIZE, 0);
	if (FS_Flags & TRIPLE_BUFFER)
		VFastClear32(FS_STscreen[2], SCREEN_SIZE, 0);
	#ifdef _CT60
	if (FS_Flags & CT60_MODE)
		VFastClear32(FS_TTscreen, SCREEN_SIZE, 0);
	#endif

	// Set a Screen and Wait VSync
	if (FS_Flags & TRIPLE_BUFFER)
		FS_NewScreenAdrs = FS_STscreen[2];
	else if (FS_Flags & DOUBLE_BUFFER)
		FS_NewScreenAdrs = FS_STscreen[1];
	else
		FS_NewScreenAdrs = FS_STscreen[0];
		
	#ifdef _CT60
	FS_VblAck = 0;
	#endif
	FS_CurrentScreen = 0;
	FS_SetNewScreen = 1;
	
	#ifdef _CT60
	while (!FS_VblAck);
	#endif

	return (NULL);
}


void	FalconLoop(int (*func)(void *screen))
{
	#ifdef _CT60
	FS_VblAck = 0;
	#endif

	while (42)
	{
		int	ret;
		
		// C2P from TTRAM to STRAM (take about 70% of the VBL @ 60Hz)
		// Optimized C2P is as fast as a raw planar 32bits copy
		// This is due to STram being very slow
		#ifdef _CT60
		if (FS_Flags & CT60_MODE)
		{
			ret = func(FS_TTscreen);
			VFastCopy32(FS_TTscreen, FS_STscreen[FS_CurrentScreen], SCREEN_SIZE);
		}
		else
		#endif
		{
			ret = func(FS_STscreen[FS_CurrentScreen]);
			FS_NewScreenAdrs = FS_STscreen[FS_CurrentScreen];
		}

		// Flip Buffers
		/*if (FS_Flags & TRIPLE_BUFFER)
		{*/
		FS_CurrentScreen++;
		if (FS_CurrentScreen == 3)
			FS_CurrentScreen = 0;
		/*}
		else if (FS_Flags & DOUBLE_BUFFER)
			FS_CurrentScreen ^= 1;*/

		// Set New Screen Flags for VBL routine
		FS_SetNewScreen = 1;

		// Wait VBL
		#ifdef _CT60
		if (FS_Flags & FORCE_WAIT_VBL)
			FS_VblAck = 0;
		while (!FS_VblAck);
		FS_VblAck = 0;
		#endif

		if (FS_Flags & EXIT_ON_SPACE_KEY)
			if (IKBD_IsKeyPressed(IKBD_KEY_ESC))
				break;
	}
}


void	FalconExit(void)
{
	// Restore CPU Flags
	#ifdef _CT60
	if ((FS_Flags & CT60_MODE) && (!(FS_Flags & EMULATOR_MODE)))
		Supexec(CpuRestoreState);
	#endif

	// Restore Interrupts
	Supexec(RestoreInterrupts);
	Supexec(IKBD_Uninstall);

	// Free Memory
	#ifdef _CT60
	if (FS_Flags & CT60_MODE)
		Mfree(FS_TTscreen);
	#endif
	if (FS_Flags & TRIPLE_BUFFER)
		Mfree(FS_STscreen[2]);
	if (FS_Flags & (DOUBLE_BUFFER | TRIPLE_BUFFER))
		Mfree(FS_STscreen[1]);
	Mfree(FS_STscreen[0]);

	// Restore Screen & Palette
	Supexec(RestoreVidel);
}

// ----

#ifdef _CT60
LONG	GetCT60Cookie(void)
{
	volatile u32 *cookie = *(u32**)0x5A0;

	while (*cookie)
	{
		if (*cookie == 0x43543630)	// 'CT60'
			return (42);
		cookie += 2;
	}

	return (0);
}
#endif

// ----
/*
u32	SetVblColorC;

LONG	SetVblColorS(void)
{
	SET_RAW_PALETTE(0, SetVblColorC);
	return (0);
}

void	SetVblColor(u32 color)
{
	SetVblColorC = color;
	Supexec(SetVblColorS);
}
*/
// ----

#define	TIMER	*((volatile unsigned int *)0x4BA)

/*
void IKBD_Flush()
{
	unsigned char	i;
	unsigned char	flush_ok;
	unsigned int	tick;

	do
	{
		tick = TIMER;
		flush_ok = (  (IKBD_MouseB    == 0)
		            &&(IKBD_Joystick0 == 0)
		            &&(IKBD_Joystick1 == 0));

		for (i = 0; i < 128; i++)
		{
			if (IKBD_Keyboard[i] != IKBD_KEY_RELEASED)
			{
				flush_ok = 0;
				break;
			}
		}
		while ((tick + 10) < TIMER);
	} while(!flush_ok);
}
*/
// ----

IKBD_MouseData	IKBD_Mouse;
/*
void	IKBD_ReadMouse(void)
{
	IKBD_Mouse.b = IKBD_MouseB;
	IKBD_Mouse.x = IKBD_Mouse.x + IKBD_MouseX;
	IKBD_Mouse.y = IKBD_Mouse.y + IKBD_MouseY;

	if (IKBD_Mouse.limit != 0)
	{
		if (IKBD_Mouse.x < 0)
			IKBD_Mouse.x = 0;
		else if (IKBD_Mouse.x >= IKBD_Mouse.w)
			IKBD_Mouse.x = IKBD_Mouse.w - 1;
		if (IKBD_Mouse.y < 0)
			IKBD_Mouse.y = 0;
		else if(IKBD_Mouse.y >= IKBD_Mouse.h)
			IKBD_Mouse.y = IKBD_Mouse.h - 1;
	}

	IKBD_MouseX = 0;
	IKBD_MouseY = 0;
}

void	IKBD_SetMouseOrigin(signed short x, signed short y, unsigned short w, unsigned short h)
{
	IKBD_Mouse.x = x;
	IKBD_Mouse.y = y;
	IKBD_Mouse.w = w;
	IKBD_Mouse.h = h;
	IKBD_Mouse.b = 0;
	IKBD_Mouse.limit = 1;
	IKBD_MouseX = 0;
	IKBD_MouseY = 0;
}
*/
