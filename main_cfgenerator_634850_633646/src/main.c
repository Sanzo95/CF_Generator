/**
 * @file main.c
 * @author Francesco Sanzone f.sanzone@studenti.uniba.it MATRICOLA: 634850
 * @author Francesco Piccinni f.piccinni5@studenti.uniba.it MATRICOLA: 633646
 * @date 05/06/2015
 *
 * Codice del progetto main_cfgenerator.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils/cfutils.h"

/**
 * Funzione main del programma per la scrittura dei codici fiscali.
 *
 * Questo programma acquisisce da riga di comando 3 argomenti:
 *  - il path assoluto del file contenente i dati anagrafici di un numero di persone
 *  - il path assoluto del file contenente i comuni italiani e i realtivi codici catastali
 *  - il path assoluto di una cartella di lavoro, che ospitera' i file di output
 *
 * Popola un file di output chiamato "people_cf.txt", creato nella cartella di lavoro, con dati anagrafici letti dal file fornito in lettura
 * aggiungendovi il codice fiscale generato.
 * Crea altri due file di output: il primo, "people_cf_males_AL.txt", nel quale sono presenti i dati inerenti le sole persone
 * di sesso maschile con cognome compreso fra A-L; ed il secondo, "people_cf_females_MZ", con i dati delle persone di sesso
 * femminile e cognome compreso fra M-Z.
 *
 * Per tale scopo utilizza tutte le funzionalitÃ  della libreria cfgenerator (importando cfutils.h)
 *
 * @param[in] argc Numero di argomenti automaticamente rilevati all'esecuzione del programma
 * @param[in] argv Array di stringhe contenente il nome del programma e gli argomenti passati
 * @return EXIT_SUCCES oppure EXIT_FAILURE
 *
 * @pre Gli argomenti passati al programma devono essere 3
 *
 */
int main(int argc, char *argv[]) {
	FILE* people = NULL;
	FILE* places = NULL;
	FILE* output_file = NULL; // people_cf.txt; people_cf_males_AL.txt; people_cf_females_MZ

	if(argc != 3+1 ){
		printf(" Gli argomenti passati al programma devono essere 3.\n");
		return EXIT_FAILURE;
	}

	char* people_cf_path = (char*) malloc(sizeof(argv[3]) * 15);
	people_cf_path = outfile_path(argv[3], "people_cf.txt");

	if ((people = fopen(argv[1], "r")) == NULL) {
		printf("Il File 'people' non può essere aperto.\n");
		return EXIT_FAILURE;
	}
	if ((places = fopen(argv[2], "r")) == NULL) {
		printf("Il File 'italian_towns' non può essere aperto.\n");
		return EXIT_FAILURE;
	}
	if ((output_file = fopen(people_cf_path, "w")) == NULL) {
		printf("Il File 'people_cf.txt' non può essere aperto.\n");
		return EXIT_FAILURE;
	} else {
		people_to_file_cf(people, output_file, places);
		fclose(output_file);
	}

	people_cf_path = outfile_path(argv[3], "people_cf_males_AL.txt");
	if ((output_file = fopen(people_cf_path, "w")) == NULL) {
		printf("Il File 'people_cf_males_AL.txt' non può essere aperto.\n");
		return EXIT_FAILURE;
	} else {
		people_to_file_cf_males_AL(people, output_file, places);
		fclose(output_file);
	}

	people_cf_path = outfile_path(argv[3], "people_cf_females_MZ.txt");
	if ((output_file = fopen(people_cf_path, "w")) == NULL) {
		printf("Il File 'people_cf_females_MZ.txt' non può essere aperto.\n");
		return EXIT_FAILURE;
	} else {
		people_to_file_cf_females_MZ(people, output_file, places);
		fclose(output_file);
	}

	fclose(places);
	fclose(people);

	//system("pause");
	return EXIT_SUCCESS;
}
