// Copyright (C) 1996-1997 Id Software, Inc. GPLv3 See LICENSE for details.

// d_part.c: software driver module for drawing particles

#include "quakedef.h"

void D_DrawParticle(particle_t *pparticle)
{
	vec3_t local, transformed;
	VectorSubtract(pparticle->org, r_origin, local); // transform point
	transformed[0] = DotProduct(local, r_pright);
	transformed[1] = DotProduct(local, r_pup);
	transformed[2] = DotProduct(local, r_ppn);
	if (transformed[2] < PARTICLE_Z_CLIP)
		return;
	f32 zi = 1.0 / transformed[2]; // project the point
	s32 u = (s32)(xcenter + zi * transformed[0] + 0.5); // FIXME: preadjust xcenter and ycenter
	s32 v = (s32)(ycenter - zi * transformed[1] + 0.5);
	if ((v > d_vrectbottom_particle) || (u > d_vrectright_particle)
		|| (v < d_vrecty) || (u < d_vrectx)) 
		return;
	s16 *pz = d_pzbuffer + (d_zwidth * v) + u;
	u8 *pdest = d_viewbuffer + d_scantable[v] + u;
	s32 izi = (s32)(zi * 0x8000);
	s32 pix;
	if (r_particlescale.value) pix = r_particlescale.value;
	else {
		pix = izi >> d_pix_shift;
		if (pix < d_pix_min) pix = d_pix_min;
		else if (pix > d_pix_max) pix = d_pix_max;
	}
	switch (pix) {
	case 1:
		for (s32 count = 1 << d_y_aspect_shift; count;
				count--, pz += d_zwidth, pdest += screenwidth)
			if (pz[0] <= izi) {
				pz[0] = izi;
				pdest[0] = pparticle->color;
			}
		break;
	case 2:
		for (s32 count = 2 << d_y_aspect_shift; count;
				count--, pz += d_zwidth, pdest += screenwidth) {
			if (pz[0] <= izi) {
				pz[0] = izi;
				pdest[0] = pparticle->color;
			}
			if (pz[1] <= izi) {
				pz[1] = izi;
				pdest[1] = pparticle->color;
			}
		}
		break;
	case 3:
		for (s32 count = 3 << d_y_aspect_shift; count;
				count--, pz += d_zwidth, pdest += screenwidth) {
			if (pz[0] <= izi) {
				pz[0] = izi;
				pdest[0] = pparticle->color;
			}
			if (pz[1] <= izi) {
				pz[1] = izi;
				pdest[1] = pparticle->color;
			}
			if (pz[2] <= izi) {
				pz[2] = izi;
				pdest[2] = pparticle->color;
			}
		}
		break;
	case 4:
		for (s32 count = 4 << d_y_aspect_shift; count;
				count--, pz += d_zwidth, pdest += screenwidth) {
			if (pz[0] <= izi) {
				pz[0] = izi;
				pdest[0] = pparticle->color;
			}
			if (pz[1] <= izi) {
				pz[1] = izi;
				pdest[1] = pparticle->color;
			}
			if (pz[2] <= izi) {
				pz[2] = izi;
				pdest[2] = pparticle->color;
			}
			if (pz[3] <= izi) {
				pz[3] = izi;
				pdest[3] = pparticle->color;
			}
		}
		break;
	default:
		for (s32 count = pix << d_y_aspect_shift; count;
				count--, pz += d_zwidth, pdest += screenwidth) {
			for (s32 i = 0; i < pix; i++) {
				if (pz[i] <= izi) {
					pz[i] = izi;
					pdest[i] = pparticle->color;
				}
			}
		}
		break;
	}
}
