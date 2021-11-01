/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "times.h"
#include "sorting.h"
#include "permutations.h"

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

/***************************************************/
/* Function: average_sorting_time Date:            */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short average_sorting_time(pfunc_sort metodo, int n_perms, int N, PTIME_AA ptime)
{
  int **perm = NULL, i = 0, ob = 0;
  clock_t time;

  perm = generate_permutations(n_perms, N);

  ptime->min_ob = INT_MAX;
  ptime->max_ob = 0;

  time = clock();
  ptime->N = N;
  for (i = 0; i < n_perms; i++)
  {
    ob = metodo(perm[i], 0, N - 1);

    if (ob > ptime->max_ob)
      ptime->max_ob = ob;

    if (ob < ptime->min_ob)
      ptime->min_ob = ob;

    ptime->average_ob += ob;
  }
  time = clock() - time;
  ptime->n_elems = n_perms;
  ptime->average_ob /= n_perms;
  ptime->time = (double)time / n_perms / CLOCKS_PER_SEC;

  for (i = 0; i < n_perms; i++)
  {
    free(perm[i]);
  }
  free(perm);

  return OK;
}

/***************************************************/
/* Function: generate_sorting_times Date:          */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short generate_sorting_times(pfunc_sort method, char *file, int num_min, int num_max, int incr, int n_perms)
{
  TIME_AA *times = NULL;
  int i = 0, tamano = 0;

  tamano = (num_max - num_min) / incr + 1;

  times = (TIME_AA *)malloc(tamano * sizeof(TIME_AA));
  if (!times)
  {
    for (i = 0; i < tamano; i++)
    {
      free(&times[i]);
    }
    free(times);
    return ERR;
  }

  for (i = 0; i < tamano; i++)
  {
    average_sorting_time(method, n_perms, i * incr + num_min, &times[i]);
  }

  save_time_table(file, times, tamano);
  free(times);
  return OK;
}

/***************************************************/
/* Function: save_time_table Date:                 */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short save_time_table(char *file, PTIME_AA ptime, int n_times)
{
  FILE *f = NULL;
  int i = 0;

  f = fopen(file, "w");

  if (f == NULL)
  {
    fclose(f);
    return ERR;
  }

  /*fprintf(f, "Tiempos de lectura:\nThe measures are: Num of permutations | Avg. time | Avg. Obs | Max | Min\n\n");*/
  for (i = 0; i < n_times; i++)
  {
    fprintf(f, "%d %f %f %d %d\n", ptime[i].N, ptime[i].time * 1000, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
  }
  fclose(f);

  return OK;
}
