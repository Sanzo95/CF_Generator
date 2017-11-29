/**
 * @file cfutils.c
 * @author Francesco Sanzone f.sanzone@studenti.uniba.it MATRICOLA: 634850
 * @author Francesco Piccinni f.piccinni5@studenti.uniba.it MATRICOLA: 633646
 * @date 03/giu/2015
 */

#include "cfutils.h"

/**
 * Controlla se il sesso di una persona e' maschile.
 *
 * @param[in] data puntatore a struttura #person contenente i dati anagrafici di una persona.
 * @return true se maschio, false altrimenti.
 *
 * @pre puntatore a struttura #person non NULL
 *
 */
bool is_male(const person* data) {
	assert(data != NULL);

	bool answer = true;

	if (data->sex != 'M') {
		answer = false;
		return answer;
	}

	return answer;
}

/**
 * Controlla se il sesso di una persona e' femminile.
 *
 * @param[in] data puntatore a struttura #person contenente i dati anagrafici di una persona.
 * @return true se donna, false altrimenti
 *
 * @pre puntatore a struttura #person non NULL
 *
 */
bool is_female(const person* data) {
	assert(data != NULL);

	bool answer = true;

	if (data->sex != 'F') {
		answer = false;
		return answer;
	}

	return answer;
}

/**
 * Controlla se il cognome di una persona inizia con una lettera compresa tra A-L
 *
 * @param[in] data puntatore a struttura #person contenente i dati anagrafici di una persona.
 * @return true se il cognome inizia con una lettera compresa tra A-L, false altrimenti
 *
 * @pre puntatore a struttura #person non NULL
 *
 */
bool is_surname_AL(const person* data) {
	assert(data != NULL);

	bool answer = true;

	if (data->surname[0] >= 'A' && data->surname[0] <= 'L') {
		return answer;
	} else {
		answer = false;
	}
	return answer;
}

/**
 * Controlla se il cognome della persona inizia con una lettera compresa tra M-Z
 *
 * @param[in] data puntatore a struttura #person contenente i dati anagrafici di una persona.
 * @return true se il cognome inizia con una lettera compresa tra M-Z, false altrimenti
 *
 * @pre puntatore a struttura #person non NULL
 */
bool is_surname_MZ(const person* data) {
	assert(data!= NULL);

	bool answer = true;

	if (data->surname[0] >= 'M' && data->surname[0] <= 'Z') {
		return answer;
	} else {
		answer = false;
	}
	return answer;
}

/**
 * Filtra le persone presenti in una lista di persone in base ad una funzione di filtraggio.
 *
 * @param[in,out] people_list puntatore a #list contenente un numero di nodi pari alle persone lette da file
 * @param[in] filter puntatore a funzione del tipo #filter_fn
 *
 * @pre puntatore a lista di persone people_list non NULL
 * @pre puntatore a funzione filter non NULL
 *
 */
void filter_people(list* people_list, filter_fn filter) {
	assert(people_list != NULL);
	assert(filter != NULL);

	node* node_list = head(people_list);
	node* node_temp = NULL;
	int i;
	int l = length(people_list);

	for (i = 0; i < l; i++) {
		if ((filter(content(node_list))) != false) {
			node_list = next(people_list, node_list);
		} else {
			node_temp = next(people_list, node_list);
			remove_node(people_list, node_list);
			delete_node(node_list);
			node_list = node_temp;
		}
	}
}

/**
 * Genera il codice fiscale delle persone presenti nella lista
 *
 * @param[in,out] people_list puntatore a #list contenente un numero di nodi pari alle persone lette da file
 * @param[in] places_file puntatore a stream in lettura collegato al file dei comuni con i codici catastali
 *
 * @pre puntatore a lista di persone people_list non NULL
 * @pre puntatore a stream places_file non NULL
 *
 */
void generate_cfs(list* people_list, FILE* places_file) {
	assert(people_list != NULL);
	assert(places_file != NULL);

	rewind(places_file);

	node* node_list = (node*) malloc(sizeof(person));
	node_list = head(people_list);
	char* bp_code;
	int l = length(people_list);
	int i;
	for (i = 0; i < l; i++) {
		bp_code = read_birth_place_code(places_file,
				(content(node_list))->birth_place);

		cf_generator((content(node_list))->surname, (content(node_list))->name,
				(content(node_list))->birth_date, bp_code, (content(node_list))->sex,
				(content(node_list))->cf);
		node_list = next(people_list, node_list);
	}
}

/**
 * Legge il codice catastale di un comune
 *
 * @param [in] places_file  puntatore a stream in lettura collegato al file dei comuni con i codici catastali
 * @param [in] birth_place stringa con il nome del comune di nascita
 * @return la stringa del codice catastale del comune di nascita
 *
 * @pre puntatore a stream places_file non NULL
 * @pre birth_place stringa non vuota
 *
 * @post il puntatore alla stringa del codice catastale ritornato non NULL
 * @post il codice catastale restituito deve essere una stringa di 4 caratteri
 *
 */
char* read_birth_place_code(FILE* places_file, const char* birth_place) {
	assert(places_file != NULL);
	assert(birth_place != NULL);

	rewind(places_file);

	int temp_char;

	char* comune = (char*) malloc(sizeof(char) * 50);

	char* code = (char*) malloc(sizeof(char) * 5);
	char* bp_code = (char*) malloc(sizeof(char) * 5);

	bool found = false;

	while ((temp_char = fgetc(places_file)) != EOF) {
		ungetc(temp_char, places_file);
		fgets(comune, 50, places_file);

		strtok(comune, ",");
		code = strtok(NULL, ",");

		if ((strcmp(birth_place, comune)) == 0) {
			strcpy(bp_code, code);
			strtok(bp_code, "\n");
			found = true;
			break;
		}
	}

	if (found == false) {
		bp_code = NULL;
	}

	assert(birth_place!=NULL);
	assert(strlen(bp_code) == 4);

	return bp_code;
}

/**
 * Legge i dati anagrafici di una singola persona come blocco di informazioni (il codice fiscale non e' presente)
 *
 * @param[in] people_file puntatore a stream in lettura collegato al file con i dati anagrafici delle persone (senza codice fiscale)
 * @return puntatore ad una struttura #person con i campi opportunamente avvalorati (il campo person#cf inizializzato a "" - stringa vuota)
 *
 * @pre puntatore a people_file non NULL
 * @post il puntatore restituito non e' NULL
 */
person* read_person(FILE* people_file) {
	assert(people_file != NULL);

	list_type dati_id = (person*) malloc(sizeof(person));

	fgets(dati_id->surname, sizeof(dati_id->surname), people_file);
	strtok(dati_id->surname, "\n");

	fgets(dati_id->name, sizeof(dati_id->name), people_file);
	strtok(dati_id->name, "\n");

	fgets(dati_id->birth_date, sizeof(dati_id->birth_date), people_file);
	strtok(dati_id->birth_date, "\n");

	fgets(dati_id->birth_place, sizeof(dati_id->birth_place), people_file);
	strtok(dati_id->birth_place, "\n");

	dati_id->sex = fgetc(people_file);
	fgetc(people_file);
	strcpy(dati_id->cf, "");

	assert(dati_id!=NULL);

	return dati_id;
}

/**
 * Legge i dati anagrafici delle persone presenti nel file di input.
 *
 * @param[in] people_file puntatore a stream in lettura collegato al file con i dati anagrafici delle persone (senza codice fiscale)
 * @return puntatore ad una struttura #list
 *
 * @pre puntatore allo stream people_file non NULL
 * @post puntatore alla lista creata non NULL
 *
 */
list* people_file_to_list(FILE* people_file) {

	assert(people_file != NULL);
	rewind(people_file);

	list* people_list = new_list();
	node* node_list = head(people_list);
	node_list = (node*) malloc(sizeof(person));

	do {
		node_list = new_node(read_person(people_file));
		add_node(people_list, tail(people_list), node_list);
	} while (!feof(people_file));

	assert(people_list!=NULL);

	return people_list;
}

/**
 * Stampa su file i dati anagrafici delle persone in lista
 *
 * @param[in] people_list puntatore a #list contenente un numero di nodi pari alle persone lette da file
 * @param[in] output_file puntatore a strem in scrittura associato ad un file
 *
 * @pre puntatore a people_list non NULL
 * @pre puntatore a output_file non NULL
 */
void list_to_file(list* people_list, FILE* output_file) {
	assert(people_list!=NULL);
	assert(output_file!=NULL);

	rewind(output_file);

	node* node_list = (node*) malloc(sizeof(person));
	node_list = head(people_list);

	int l = length(people_list);

	for (int i = 0; i < l; i++) {
		fputs(content(node_list)->surname, output_file);
		fputs("\n", output_file);
		fputs(content(node_list)->name, output_file);
		fputs("\n", output_file);
		fputs(content(node_list)->birth_date, output_file);
		fputs("\n", output_file);
		fputs(content(node_list)->birth_place, output_file);
		fputs("\n", output_file);
		fputc(content(node_list)->sex, output_file);
		fputs("\n", output_file);
		fputs(content(node_list)->cf, output_file);
		fputs("\n", output_file);

		node_list = next(people_list, node_list);
	}
}

/**
 * Filtra una lista preservando individui di sesso maschile con cognome compreso tra A-L
 *
 * @param[in] people_list puntatore a #list contenente un numero di nodi pari alle persone lette da file
 * @param[in] output_file puntatore a strem in scrittura associato ad un file
 *
 *
 * @pre puntatore a people_list non NULL
 * @pre puntatore a output_file non NULL
 */
void list_to_file_males_AL(list* people_list, FILE* output_file) {
	assert(people_list!=NULL);
	assert(output_file!=NULL);
	rewind(output_file);

	filter_people(people_list, is_male);
	filter_people(people_list, is_surname_AL);
	list_to_file(people_list, output_file);
}

/**
 * Filtra una lista preservando individui di sesso femminile con cognome compreso tra M-Z
 *
 * @param[in] people_list puntatore a #list contenente un numero di nodi pari alle persone lette da file
 * @param[in] output_file puntatore a strem in scrittura associato ad un file
 *
 * @pre puntatore a people_list non NULL
 * @pre puntatore a output_file non NULL
 */
void list_to_file_females_MZ(list* people_list, FILE* output_file) {
	assert(people_list!=NULL);
	assert(output_file!=NULL);
	rewind(output_file);

	filter_people(people_list, is_female);
	filter_people(people_list, is_surname_MZ);
	list_to_file(people_list, output_file);
}

/**
 * Legge i dati anagrafici di persone da un file e li riscrive in un altro file aggiungendovi il codice fiscale
 *
 * @param[in] people_file
 * @param[in] output_file
 * @param[in] places_file
 *
 * @pre il puntatore allo stream people_file non deve essere NULL
 * @pre il puntatore allo stream output_file non deve essere NULL
 * @pre il puntatore allo stream places_file non deve essere NULL
 *
 */
void people_to_file_cf(FILE* people_file, FILE* output_file, FILE* places_file) {
	assert(people_file!=NULL);
	assert(output_file!=NULL);
	assert(places_file!=NULL);

	rewind(people_file);
	rewind(output_file);
	rewind(places_file);

	list* people_list;

	people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file(people_list, output_file);
}

/**
 * Legge i dati anagrafici di persone da un file, filtra i risultati e li riscrive in un altro file aggiungendovi il codice fiscale
 *
 * @param[in] people_file
 * @param[in] output_file
 * @param[in] places_file
 *
 * @pre il puntatore allo stream people_file non deve essere NULL
 * @pre il puntatore allo stream output_file non deve essere NULL
 * @pre il puntatore allo stream places_file non deve essere NULL
 *
 */
void people_to_file_cf_males_AL(FILE* people_file, FILE* output_file,
		FILE* places_file) {
	assert(people_file!=NULL);
	assert(output_file!=NULL);
	assert(places_file!=NULL);

	rewind(people_file);
	rewind(output_file);
	rewind(places_file);

	list* people_list;

	people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file_males_AL(people_list, output_file);
}
/**
 * Legge i dati anagrafici di persone da un file, filtra i risultati e li riscrive in un altro file aggiungendovi il codice fiscale
 *
 * @param[in] people_file
 * @param[in] output_file
 * @param[in] places_file
 *
 * @pre il puntatore allo stream people_list non deve essere NULL
 * @pre il puntatore allo stream output_file non deve essere NULL
 * @pre il puntatore allo stream places_file non deve essere NULL
 *
 */
void people_to_file_cf_females_MZ(FILE* people_file, FILE* output_file,
		FILE* places_file) {
	assert(people_file!=NULL);
	assert(output_file!=NULL);
	assert(places_file!=NULL);

	rewind(people_file);
	rewind(output_file);
	rewind(places_file);

	list* people_list;

	people_list = people_file_to_list(people_file);
	generate_cfs(people_list, places_file);
	list_to_file_females_MZ(people_list, output_file);
}

/**
 * Genera il path assoluto di filename
 *
 * @param[in] wd_path path assoluto della directory di lavoro in cui salvare i file di output
 * @param[in] filename nome del file di output
 * @return path assoluto del filename
 *
 * @pre wd_path deve terminare con il carattere separatore (\\ per Win e / per linux) da specificare in #OS_SEP
 *
 * @post la lunghezza del path ritornato deve essere somma delle lunghezze di wd_path e filename
 *
 */
char* outfile_path(char* wd_path, char* filename) {

	assert(wd_path [ strlen(wd_path) - 1 ] == OS_SEP);

	char * filepath = (char*) malloc((strlen(wd_path) + strlen(filename) + 1) * sizeof(char));

	//strcpy(filepath, "");
	strcpy(filepath, wd_path);
	strcat(filepath, filename);
	assert(strlen(filepath) == strlen(wd_path) + strlen(filename));

	return filepath;
}
