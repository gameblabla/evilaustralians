#pragma once

// Cel Control Block Chunk
struct sCCB
{
	int		chunk_ID;			// 'CCB ' Magic number to identify pixel data
	int		chunk_size; 		// size in bytes of chunk including chunk_size
	DWORD	ccbversion; 		// version number.  0 for now
	DWORD	ccb_Flags;			// 32 bits of CCB flags
	void	*ccb_NextPtr;
	void	*ccb_CelData;
	void	*ccb_PIPPtr;

	int		ccb_X;
	int		ccb_Y;
	int		ccb_hdx;
	int		ccb_hdy;
	int		ccb_vdx;
	int		ccb_vdy;
	int		ccb_ddx;
	int		ccb_ddy;
	DWORD	ccb_PPMPC;
	DWORD	ccb_PRE0;			// Sprite Preamble Word 0
	DWORD	ccb_PRE1;			// Sprite Preamble Word 1
	int		ccb_Width;
	int		ccb_Height;
};
