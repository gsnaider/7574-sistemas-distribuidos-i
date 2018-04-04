/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "sumar.h"

#include <stdio.h>
#include <time.h>


void
sumar_prg_1(char *host, int arg)
{
	CLIENT *clnt;
	int  *result_1;
	int  sumar_1_arg = arg;

#ifndef	DEBUG
	clnt = clnt_create (host, sumar_prg, sumar_ver, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = sumar_1(&sumar_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void sumar_local(int n) {
	printf("%d\n", n+1);
}


int
main (int argc, char *argv[])
{
	char *host;
	int n;

	if (argc < 3) {
		printf ("usage: %s server_host number\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	n = atoi(argv[2]);

	// Local
	clock_t t0 = clock();
	sumar_local (n);
	clock_t t1 = clock();
	float lapse = (float)(t1 - t0) / CLOCKS_PER_SEC;
	printf("Función local tardó: %lf\n", lapse);

	// Remoto
	printf("Llamo con %d\n", n);
	t0 = clock();
	sumar_prg_1 (host, n);
	t1 = clock();
	lapse = (float)(t1 - t0) / CLOCKS_PER_SEC;
	printf("Volví del remoto. Tardó %lf\n", lapse);

exit (0);
}
