/*
 * Copyright (c) 2002, 2016 Jens Keiner, Stefan Kunis, Daniel Potts
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define NFFT_PRECISION_DOUBLE

#include "nfft3mp.h"

static void simple_test_nfct_1d(void)
{
  NFCT(plan) p;

  const char *error_str;

  int N = 14;
  int M = 19;

  /** init an one dimensional plan */
  NFCT(init_1d)(&p,N,M);

  /** init pseudo random nodes */
  NFFT(vrand_real)(p.x, p.M_total, NFFT_K(0.0), NFFT_K(0.5));

  /** precompute psi, the entries of the matrix B */
  if( p.flags & PRE_ONE_PSI)
    NFCT(precompute_one_psi)(&p);

  /** init pseudo random Fourier coefficients and show them */
  NFFT(vrand_real)(p.f_hat, p.N_total, NFFT_K(0.0), NFFT_K(1.0));
  NFFT(vpr_double)(p.f_hat,p.N_total,"given Fourier coefficients, vector f_hat");

  /** check for valid parameters before calling any trafo/adjoint method */
  error_str = NFCT(check)(&p);
  if (error_str != 0)
  {
    printf("Error in nfct module: %s\n", error_str);
    return;
  }

  /** direct trafo and show the result */
  NFCT(trafo_direct)(&p);
  NFFT(vpr_double)(p.f,p.M_total,"ndct, vector f");

  /** approx. trafo and show the result */
  NFCT(trafo)(&p);
  NFFT(vpr_double)(p.f,p.M_total,"nfct, vector f");

  /** approx. adjoint and show the result */
  NFCT(adjoint_direct)(&p);
  NFFT(vpr_double)(p.f_hat,p.N_total,"adjoint ndct, vector f_hat");

  /** approx. adjoint and show the result */
  NFCT(adjoint)(&p);
  NFFT(vpr_double)(p.f_hat,p.N_total,"adjoint nfct, vector f_hat");

  /** finalise the one dimensional plan */
  NFCT(finalize)(&p);
}

int main(void)
{
  printf("Computing one dimensional ndct, nfct, adjoint ndct, and adjoint nfct...\n\n");
  simple_test_nfct_1d();
  printf("\n\n");

  return EXIT_SUCCESS;
}
