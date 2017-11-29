/**
 * @file test_cfutils_634850_633646.c
 * @author Francesco Sanzone f.sanzone@studenti.uniba.it MATRICOLA: 634850
 * @author Francesco Piccinni f.piccinni5@studenti.uniba.it MATRICOLA: 633646
 * @date 09/06/2015
 */

#include "utils/cfutils.h"
#include "CUnit/Basic.h"

/**
 *
 * Verifica che il contenuto di una lista di persone sia uguale a quello del file originario.
 *
 * Data una lista di nodi del tipo #node la scorre confrontando i nodi con il valore atteso.
 * Viene richiamata nelle funzioni che testano le persone, ovvero #test_generate_cfs e #test_people_to_file_cf
 *
 * @param list_people Lista di persone
 */
void ctrl_full_FILE(list* list_people);

/**
 * Verifica che il contenuto di una lista filtrata di persone sia uguale a quello del file originario (filtrato).
 *
 * Data una lista di nodi del tipo #node la scorre confrontando i nodi con il valore atteso.
 * Viene richiamata nella funzione #test_people_to_file_cf_males_AL
 * che testa le persone filtrate con cognome compreso tra A-L e sesso maschile.
 *
 * @param list_people Lista di persone
 */
void ctrl_FILE_AL_male(list* list_people);

/**
 * Verifica che il contenuto di una lista filtrata di persone sia uguale a quello del file originario (filtrato).
 *
 * Data una lista di nodi del tipo #node la scorre confrontando i nodi con il valore atteso.
 * Viene richiamata nella funzione #test_people_to_file_cf_females_MZ
 * che testa le persone filtrate con cognome compreso tra M-Z e sesso femminile.
 *
 * @param list_people Lista di persone
 */
void ctrl_FILE_MZ_female(list* list_people);

void test_is_male(void) {
	list_type dati_id = (person*) malloc(sizeof(person));
	CU_ASSERT_FALSE(dati_id == NULL);
	dati_id->sex = 'M';
	CU_ASSERT_TRUE(is_male(dati_id));
	free(dati_id);
	dati_id->sex = 'F';
	CU_ASSERT_FALSE(is_male(dati_id));
	free(dati_id);
}

void test_is_female(void) {
	list_type dati_id = (person*) malloc(sizeof(person));

	dati_id->sex = 'M';
	CU_ASSERT_FALSE(is_female(dati_id));
	free(dati_id);

	dati_id->sex = 'F';
	CU_ASSERT_TRUE(is_female(dati_id));
	free(dati_id);
}

void test_is_surname_AL(void) {
	list_type dati_id = (person*) malloc(sizeof(person));

	strcpy(dati_id->surname, "ABATANGELO");
	CU_ASSERT_TRUE(is_surname_AL(dati_id));

	strcpy(dati_id->surname, "LOMORO");
	CU_ASSERT_TRUE(is_surname_AL(dati_id));

	strcpy(dati_id->surname, "MACHIAVELLI");
	CU_ASSERT_FALSE(is_surname_AL(dati_id));

	strcpy(dati_id->surname, "ZACCARO");
	CU_ASSERT_FALSE(is_surname_AL(dati_id));

	free(dati_id);
}

void test_is_surname_MZ(void) {
	list_type dati_id = (person*) malloc(sizeof(person));

	strcpy(dati_id->surname, "MACHIAVELLI");
	CU_ASSERT_TRUE(is_surname_MZ(dati_id));

	strcpy(dati_id->surname, "ZACCARO");
	CU_ASSERT_TRUE(is_surname_MZ(dati_id));

	strcpy(dati_id->surname, "ABATANGELO");
	CU_ASSERT_FALSE(is_surname_MZ(dati_id));

	strcpy(dati_id->surname, "LOMORO");
	CU_ASSERT_FALSE(is_surname_MZ(dati_id));

	free(dati_id);
}

void test_filter_people(void) {
	FILE* people = fopen("people.txt", "r");
	list* people_list = people_file_to_list(people);
	filter_people(people_list, is_male);

	node* node_list = head(people_list);
	list_type dati_id;

	while (node_list != NULL) {
		dati_id = content(node_list);
		CU_ASSERT_TRUE(is_male(dati_id));
		node_list = next(people_list, node_list);
	}

	rewind(people);
	people_list = people_file_to_list(people);
	filter_people(people_list, is_female);
	node_list = head(people_list);
	while (node_list != NULL) {
		dati_id = content(node_list);
		CU_ASSERT_TRUE(is_female(dati_id));
		node_list = next(people_list, node_list);
	}

	rewind(people);
	people_list = people_file_to_list(people);
	filter_people(people_list, is_surname_AL);
	node_list = head(people_list);
	while (node_list != NULL) {
		dati_id = content(node_list);
		CU_ASSERT_TRUE(is_surname_AL(dati_id));
		node_list = next(people_list, node_list);
	}

	rewind(people);
	people_list = people_file_to_list(people);
	filter_people(people_list, is_surname_MZ);
	node_list = head(people_list);
	while (node_list != NULL) {
		dati_id = content(node_list);
		CU_ASSERT_TRUE(is_surname_MZ(dati_id));
		node_list = next(people_list, node_list);
	}
	fclose(people);
	delete_list(people_list);
}

void test_generate_cfs(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* places = fopen("italian_towns.txt", "r");
	list* people_list = people_file_to_list(people);

	generate_cfs(people_list, places);
	ctrl_full_FILE(people_list);

	fclose(people);
	fclose(places);
	delete_list(people_list);

}

void test_read_birth_place_code(void) {
	FILE* places = fopen("italian_towns.txt", "r");
	char* bp_code;

	bp_code = read_birth_place_code(places, "BARI");
	CU_ASSERT_STRING_EQUAL(bp_code, "A662");

	bp_code = read_birth_place_code(places, "ROMA");
	CU_ASSERT_STRING_EQUAL(bp_code, "H501");

	bp_code = read_birth_place_code(places, "SAN SECONDO PARMENSE");
	CU_ASSERT_STRING_EQUAL(bp_code, "I153");

	bp_code = NULL;
	CU_ASSERT_PTR_NULL(bp_code);

	fclose(places);

}

void test_read_person(void) {
	FILE* people = fopen("people.txt", "r");
	list_type dati_id = read_person(people);

	list* people_list = people_file_to_list(people);
	node* node_list = head(people_list);

	CU_ASSERT_PTR_NOT_NULL(dati_id);
	CU_ASSERT_STRING_EQUAL(dati_id->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_id->name, "LINO");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_date, "01/02/1903");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_place, "ROMA");
	CU_ASSERT_EQUAL(dati_id->sex, 'M');

	node_list = next(people_list, node_list);
	dati_id = content(node_list);
	CU_ASSERT_PTR_NOT_NULL(dati_id);
	CU_ASSERT_STRING_EQUAL(dati_id->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_id->name, "LINA");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_date, "03/05/1903");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_place, "GENOVA");
	CU_ASSERT_EQUAL(dati_id->sex, 'F');

	node_list = tail(people_list);
	dati_id = content(node_list);
	CU_ASSERT_PTR_NOT_NULL(dati_id);
	CU_ASSERT_STRING_EQUAL(dati_id->surname, "LARA");
	CU_ASSERT_STRING_EQUAL(dati_id->name, "CROFT");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_date, "26/04/1981");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_place, "BOLZANO");
	CU_ASSERT_EQUAL(dati_id->sex, 'F');

	node_list = lookfor(people_list, dati_id);
	CU_ASSERT_NOT_EQUAL(node_list, NULL);

	node_list = next(people_list, node_list);
	CU_ASSERT_EQUAL(node_list, NULL);

	fclose(people);
}

void test_people_file_to_list(void) {
	FILE* people = fopen("people.txt", "r");

	CU_ASSERT_PTR_NOT_NULL(people);

	list* people_list = new_list();
	CU_ASSERT_TRUE(length(people_list) == 0);
	CU_ASSERT_PTR_NOT_NULL(people_list);

	list_type dati_id;
	node* node_list;

	people_list = people_file_to_list(people);

	node_list = head(people_list);
	dati_id = content(node_list);
	CU_ASSERT_PTR_NOT_NULL(node_list);
	CU_ASSERT_STRING_EQUAL(dati_id->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_id->name, "LINO");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_date, "01/02/1903");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_place, "ROMA");
	CU_ASSERT_TRUE(dati_id->sex == 'M');

	node_list = tail(people_list);
	CU_ASSERT_PTR_NOT_NULL(node_list);
	dati_id = content(node_list);
	CU_ASSERT_PTR_NOT_NULL(node_list);
	CU_ASSERT_STRING_EQUAL(dati_id->surname, "LARA");
	CU_ASSERT_STRING_EQUAL(dati_id->name, "CROFT");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_date, "26/04/1981");
	CU_ASSERT_STRING_EQUAL(dati_id->birth_place, "BOLZANO");
	CU_ASSERT_TRUE(dati_id->sex == 'F');

	node_list = next(people_list, node_list);
	CU_ASSERT_PTR_NULL(node_list);
	delete_list(people_list);

	fclose(people);
}

void test_list_to_file(void) {
	FILE* people = fopen("people.txt", "r");
	list* people_list = people_file_to_list(people);
	FILE* testPtr = fopen("people_cf_test.txt", "w");
	list_to_file(people_list, testPtr);
	fclose(people);
	fclose(testPtr);
	testPtr = fopen("people_cf_test.txt", "r");

	list_type dati_id;
	node* node_list = head(people_list);
	char* stringa = (char*) malloc(sizeof(char) * 50);
	char temp_sex;
	int temp;
	dati_id = content(node_list);

	strcat(dati_id->surname, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->surname);

	strcat(dati_id->name, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->name);

	strcat(dati_id->birth_date, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->birth_date);

	strcat(dati_id->birth_place, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->birth_place);

	fscanf(testPtr, "%c", &temp_sex);
	CU_ASSERT_PTR_EQUAL(temp_sex, dati_id->sex);
	fgets(stringa, 50, testPtr);

	node_list = tail(people_list);
	dati_id = content(node_list);

	temp = ftell(testPtr) + 1;

	fseek(testPtr, -(temp + strlen(dati_id->surname) + 1), SEEK_END);

	strcat(dati_id->surname, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->surname);

	strcat(dati_id->name, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->name);

	strcat(dati_id->birth_date, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->birth_date);

	strcat(dati_id->birth_place, "\n");
	fgets(stringa, 50, testPtr);
	CU_ASSERT_STRING_EQUAL(stringa, dati_id->birth_place);

	fscanf(testPtr, "%c", &temp_sex);
	CU_ASSERT_PTR_EQUAL(temp_sex, dati_id->sex);
	fgets(stringa, 50, testPtr);
	fgets(stringa, 50, testPtr);

	node_list = next(people_list, node_list);
	CU_ASSERT_PTR_NULL(node_list);

	temp = fgetc(testPtr);
	CU_ASSERT_TRUE(temp == EOF);

	delete_list(people_list);

	fclose(testPtr);
}

void test_list_to_file_males_AL(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* testPtr = fopen("people_cf_males_AL_test.txt", "w");
	list* people_list = people_file_to_list(people);
	list_to_file_males_AL(people_list, testPtr);
	fclose(people);
	fclose(testPtr);

	testPtr = fopen("people_cf_males_AL_test.txt", "r");
	rewind(testPtr);
	char* stringa = (char*) malloc(sizeof(char) * 50);
	node* node_list = head(people_list);
	list_type dati_id;

	dati_id = content(node_list);
	CU_ASSERT_TRUE(is_surname_AL(dati_id));
	CU_ASSERT_TRUE(is_male(dati_id));

	fgets(stringa, 50, testPtr); //legge srn
	CU_ASSERT_TRUE(stringa[0] >= 'A' && stringa[0] <= 'L');
	fgets(stringa, 50, testPtr); //legge name
	fgets(stringa, 50, testPtr); //legge bd
	fgets(stringa, 50, testPtr); //legge bp
	fgets(stringa, 50, testPtr); //legge sex
	CU_ASSERT_EQUAL(stringa[0], 'M');

	fgets(stringa, 50, testPtr); //legge cf

	node_list = tail(people_list);
	dati_id = content(node_list);
	CU_ASSERT_TRUE(is_surname_AL(dati_id));
	CU_ASSERT_TRUE(is_male(dati_id));

	fclose(testPtr);

	delete_list(people_list);
}

void test_list_to_file_females_MZ(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* testPtr = fopen("people_cf_females_MZ_test.txt", "w");

	list* people_list = people_file_to_list(people);
	list_to_file_females_MZ(people_list, testPtr);

	fclose(people);
	fclose(testPtr);

	testPtr = fopen("people_cf_females_MZ_test.txt", "r");

	char* stringa = (char*) malloc(sizeof(char) * 50);
	node* node_list = head(people_list);
	list_type dati_id;

	dati_id = content(node_list);
	CU_ASSERT_TRUE(is_surname_MZ(dati_id));
	CU_ASSERT_TRUE(is_female(dati_id));

	fgets(stringa, 50, testPtr); //legge srn
	CU_ASSERT_TRUE(stringa[0] >= 'M' && stringa[0] <= 'Z');
	fgets(stringa, 50, testPtr); //legge name
	fgets(stringa, 50, testPtr); //legge bd
	fgets(stringa, 50, testPtr); //legge bp
	fgets(stringa, 50, testPtr); //legge sex
	CU_ASSERT_EQUAL(stringa[0], 'F');

	node_list = tail(people_list);
	dati_id = content(node_list);
	CU_ASSERT_TRUE(is_surname_MZ(dati_id));
	CU_ASSERT_TRUE(is_female(dati_id));

	fclose(testPtr);

	delete_list(people_list);
}

void test_people_to_file_cf(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* places = fopen("italian_towns.txt", "r");
	FILE* testPtr = NULL;

	list* people_list = NULL;

	node* node_list = NULL;
	node* node_temp = NULL;

	list_type dati_id = NULL;

	if ((testPtr = fopen("people_cf_test.txt", "w")) == NULL) {
		printf("File could not be  opened\n");
	} //fine del ramo if
	else {
		people_to_file_cf(people, testPtr, places);
		fclose(testPtr);
	} //fine del ramo else

	testPtr = fopen("people_cf_test.txt", "r");

	dati_id = (person*) malloc(sizeof(person));

	fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
	strtok(dati_id->surname, "\n");
	fgets(dati_id->name, sizeof(dati_id->name), testPtr);
	strtok(dati_id->name, "\n");
	fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
	strtok(dati_id->birth_date, "\n");
	fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
	strtok(dati_id->birth_place, "\n");
	dati_id->sex = fgetc(testPtr);
	fgetc(testPtr);
	fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
	strtok(dati_id->cf, "\n");
	node_list = new_node(dati_id);
	people_list = new_list();
	add_node(people_list, NULL, node_list);
	do {
		fgetc(testPtr);
		dati_id = (person*) malloc(sizeof(person));
		fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
		strtok(dati_id->surname, "\n");
		fgets(dati_id->name, sizeof(dati_id->name), testPtr);
		strtok(dati_id->name, "\n");
		fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
		strtok(dati_id->birth_date, "\n");
		fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
		strtok(dati_id->birth_place, "\n");
		dati_id->sex = fgetc(testPtr);
		fgetc(testPtr);
		fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
		strtok(dati_id->cf, "\n");
		node_temp = node_list;
		node_list = new_node(dati_id);
		add_node(people_list, node_temp, node_list);
	} while (!feof(testPtr));
	remove_node(people_list, node_list);
	delete_node(node_list);
	ctrl_full_FILE(people_list);

	fclose(testPtr);
	fclose(places);
	fclose(people);
	delete_list(people_list);
}

void test_people_to_file_cf_males_AL(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* places = fopen("italian_towns.txt", "r");
	FILE* testPtr = NULL;

	if ((testPtr = fopen("people_cf_males_AL_test.txt", "w")) == NULL) {
		printf("File could not be  opened\n");
	} //fine del ramo if
	else {
		people_to_file_cf_males_AL(people, testPtr, places);
		fclose(testPtr);
	} //fine del ramo else
	list* people_list = NULL;
	node* node_list = NULL;
	node* node_temp = NULL;
	list_type dati_id = NULL;
	dati_id = (person*) malloc(sizeof(person));

	testPtr = fopen("people_cf_males_AL_test.txt", "r");
	fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
	strtok(dati_id->surname, "\n");
	fgets(dati_id->name, sizeof(dati_id->name), testPtr);
	strtok(dati_id->name, "\n");
	fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
	strtok(dati_id->birth_date, "\n");
	fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
	strtok(dati_id->birth_place, "\n");
	dati_id->sex = fgetc(testPtr);
	fgetc(testPtr);
	fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
	strtok(dati_id->cf, "\n");
	node_list = new_node(dati_id);
	people_list = new_list();
	add_node(people_list, NULL, node_list);
	while (!feof(testPtr)) {
		fgetc(testPtr);
		dati_id = (person*) malloc(sizeof(person));
		fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
		strtok(dati_id->surname, "\n");
		fgets(dati_id->name, sizeof(dati_id->name), testPtr);
		strtok(dati_id->name, "\n");
		fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
		strtok(dati_id->birth_date, "\n");
		fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
		strtok(dati_id->birth_place, "\n");
		dati_id->sex = fgetc(testPtr);
		fgetc(testPtr);
		fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
		strtok(dati_id->cf, "\n");
		node_temp = node_list;
		node_list = new_node(dati_id);
		add_node(people_list, node_temp, node_list);
	}
	remove_node(people_list, node_list);
	delete_node(node_list);
	ctrl_FILE_AL_male(people_list);

	fclose(testPtr);
	fclose(places);
	fclose(people);

	delete_list(people_list);
}

void test_people_to_file_cf_females_MZ(void) {
	FILE* people = fopen("people.txt", "r");
	FILE* places = fopen("italian_towns.txt", "r");
	FILE* testPtr = NULL;

	if ((testPtr = fopen("people_cf_females_MZ_test.txt", "w")) == NULL) {
		printf("File could not be  opened\n");
	} //fine del ramo if
	else {
		people_to_file_cf_females_MZ(people, testPtr, places);
		fclose(testPtr);
	} //fine del ramo else
	list* people_list = NULL;
	node* node_list = NULL;
	node* node_temp = NULL;
	list_type dati_id = NULL;
	dati_id = (person*) malloc(sizeof(person));

	testPtr = fopen("people_cf_females_MZ_test.txt", "r");
	fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
	strtok(dati_id->surname, "\n");
	fgets(dati_id->name, sizeof(dati_id->name), testPtr);
	strtok(dati_id->name, "\n");
	fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
	strtok(dati_id->birth_date, "\n");
	fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
	strtok(dati_id->birth_place, "\n");
	dati_id->sex = fgetc(testPtr);
	fgetc(testPtr);
	fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
	strtok(dati_id->cf, "\n");
	node_list = new_node(dati_id);
	people_list = new_list();
	add_node(people_list, NULL, node_list);

	while (!feof(testPtr)) {
		fgetc(testPtr);
		dati_id = (person*) malloc(sizeof(person));
		fgets(dati_id->surname, sizeof(dati_id->surname), testPtr);
		strtok(dati_id->surname, "\n");
		fgets(dati_id->name, sizeof(dati_id->name), testPtr);
		strtok(dati_id->name, "\n");
		fgets(dati_id->birth_date, sizeof(dati_id->birth_date), testPtr);
		strtok(dati_id->birth_date, "\n");
		fgets(dati_id->birth_place, sizeof(dati_id->birth_place), testPtr);
		strtok(dati_id->birth_place, "\n");
		dati_id->sex = fgetc(testPtr);
		fgetc(testPtr);
		fgets(dati_id->cf, sizeof(dati_id->cf), testPtr);
		strtok(dati_id->cf, "\n");
		node_temp = node_list;
		node_list = new_node(dati_id);
		add_node(people_list, node_temp, node_list);
	}

	remove_node(people_list, node_list);
	delete_node(node_list);
	ctrl_FILE_MZ_female(people_list);

	fclose(testPtr);
	fclose(places);
	fclose(people);
	delete_list(people_list);
}

void test_outfile_path(void) {
	if(OS_SEP == '/'){
		char* wd_path = "/path/wdir/";
		CU_ASSERT_TRUE(wd_path [ strlen(wd_path) - 1 ] == OS_SEP);
		char* filename = "file.txt";
		char* path = outfile_path(wd_path, filename);
		char* wd_path_temp = (char*) malloc((strlen(wd_path) + strlen(filename) + 1)* sizeof(char));

		strcpy(wd_path_temp, "");
		strcpy(wd_path_temp, wd_path);
		strcat(wd_path_temp, filename);

		CU_ASSERT_STRING_EQUAL(path, wd_path_temp);
		CU_ASSERT_TRUE(strlen(path) == strlen(wd_path) + strlen(filename));
	}
	else if(OS_SEP == '\\')
	{
		char* wd_path = "C:\\path\\wdir\\";
		CU_ASSERT_TRUE(wd_path [ strlen(wd_path) - 1 ] == OS_SEP);
		char* filename = "file.txt";
		char* path = outfile_path(wd_path, filename);
		char* wd_path_temp = (char*) malloc((strlen(wd_path) + strlen(filename) + 1)* sizeof(char));

		strcpy(wd_path_temp, "");
		strcpy(wd_path_temp, wd_path);
		strcat(wd_path_temp, filename);

		CU_ASSERT_STRING_EQUAL(path, wd_path_temp);
		CU_ASSERT_TRUE(strlen(path) == strlen(wd_path) + strlen(filename));
	}
	else
	{
		CU_FAIL();
	}
}

/*
 * Funzioni di inizializzazione e pulizia delle suite.
 * Di default sono funzioni vuote.
 */
int init_suite_default(void) {
	return 0;
}

int clean_suite_default(void) {
	return 0;
}

int main() {
	/* inizializza registro - e' la prima istruzione */
	CU_initialize_registry();

	/* Aggiungi le suite al test registry */

	CU_pSuite pSuite_Bool = CU_add_suite("Suite_Boolean", init_suite_default,
			clean_suite_default);
	CU_pSuite pSuite_Filter = CU_add_suite("Suite_Filter", init_suite_default,
			clean_suite_default);
	CU_pSuite pSuite_CF_Generator = CU_add_suite("Suite_CF_Generator",
			init_suite_default, clean_suite_default);
	CU_pSuite pSuite_Read = CU_add_suite("Suite_Read", init_suite_default,
			clean_suite_default);
	CU_pSuite pSuite_File_to_List = CU_add_suite("Suite_File_to_List",
			init_suite_default, clean_suite_default);
	CU_pSuite pSuite_List_to_File = CU_add_suite("Suite_List_to_File",
			init_suite_default, clean_suite_default);
	CU_pSuite pSuite_People_to_File = CU_add_suite("Suite_People_to_File",
			init_suite_default, clean_suite_default);
	CU_pSuite pSuite_Outfile = CU_add_suite("Suite_Outfile_Path",
			init_suite_default, clean_suite_default);

	CU_add_test(pSuite_Bool, "test of is_male()", test_is_male);
	CU_add_test(pSuite_Bool, "test of is_female()", test_is_female);
	CU_add_test(pSuite_Bool, "test of is_surname_AL()", test_is_surname_AL);
	CU_add_test(pSuite_Bool, "test of is_surname_MZ()", test_is_surname_MZ);

	CU_add_test(pSuite_Filter, "test of filter_people()", test_filter_people);
	CU_add_test(pSuite_CF_Generator, "test of generate_cfs()",
			test_generate_cfs);

	CU_add_test(pSuite_Read, "test of read_birth_place_code()",
			test_read_birth_place_code);
	CU_add_test(pSuite_Read, "test of read_person()", test_read_person);

	CU_add_test(pSuite_File_to_List, "test of people_file_to_list()",
			test_people_file_to_list);

	CU_add_test(pSuite_List_to_File, "test of list_to_file()",
			test_list_to_file);
	CU_add_test(pSuite_List_to_File, "test of list_to_file_males_AL()",
			test_list_to_file_males_AL);
	CU_add_test(pSuite_List_to_File, "test of list_to_file_females_MZ()",
			test_list_to_file_females_MZ);

	CU_add_test(pSuite_People_to_File, "test of people_to_file_cf()",
			test_people_to_file_cf);
	CU_add_test(pSuite_People_to_File, "test of people_to_file_cf_males_AL()",
			test_people_to_file_cf_males_AL);
	CU_add_test(pSuite_People_to_File, "test of people_to_file_cf_females_MZ()",
			test_people_to_file_cf_females_MZ);

	CU_add_test(pSuite_Outfile, "test of outfile_path()", test_outfile_path);

	/* Esegue tutti i casi di test con output sulla console */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	/* Pulisce il registro e termina lo unit test */
	CU_cleanup_registry();

	return CU_get_error();
}

void ctrl_full_FILE(list* list_people) {

	node* nodo_persona = head(list_people);
	list_type dati_identificativi;

	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "LINO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "01/02/1903");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "ROMA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "TPOLNI03B01H501Z");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "LINA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "03/05/1903");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GENOVA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "TPOLNI03E43D969Y");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "PI");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "PPO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "25/04/1958");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "ANCONA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "PIXPPO58D25A271T");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "PLU");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "UTO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "16/12/2005");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "MILANO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "PLUTUO05T16F205I");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "PAPE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "RINA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "18/10/1946");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "TRENTO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "PPARNI46R58L378Z");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "PA PER");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "INO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "19/03/1994");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "UDINE");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "PPRNIO94C19L483D");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "ARSENE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "LUPIN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "20/10/1905");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "PALERMO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "RSNLPN05R20G273L");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "MINE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "FUJIKO MARGOT");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "25/08/1980");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "CATANIA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "MNIFKM80M65C351J");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "ISHIKAWA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "GOEMON");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "15/12/1986");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GIOIA DEL COLLE");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "SHKGMN86T15E038I");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "DAISUKE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "JIGEN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "16/06/1981");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GAETA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "DSKJGN81H16D843U");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "KOICHI");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "ZENIGATA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "14/03/1979");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "BOLOGNA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "KCHZGT79C14A944Z");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "LA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "MUU");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "15/03/1978");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place,
			"CASALECCHIO DI RENO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "LAXMUU78C55B880Y");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "CAPITAN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "HARLOCK");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "09/07/1972");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "VENEZIA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "CPTHLC72L09L736H");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "LARA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "CROFT");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "26/04/1981");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "BOLZANO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "LRACFT81D66A952F");

	CU_ASSERT_PTR_NULL(next(list_people, nodo_persona));
}

void ctrl_FILE_AL_male(list* list_people) {
	node* nodo_persona = head(list_people);
	list_type dati_identificativi;

	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "ARSENE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "LUPIN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "20/10/1905");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "PALERMO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "RSNLPN05R20G273L");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "ISHIKAWA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "GOEMON");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "15/12/1986");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GIOIA DEL COLLE");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "SHKGMN86T15E038I");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "DAISUKE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "JIGEN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "16/06/1981");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GAETA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "DSKJGN81H16D843U");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "KOICHI");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "ZENIGATA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "14/03/1979");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "BOLOGNA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "KCHZGT79C14A944Z");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "CAPITAN");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "HARLOCK");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "09/07/1972");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "VENEZIA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'M');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "CPTHLC72L09L736H");

	CU_ASSERT_PTR_NULL(next(list_people, nodo_persona));
}

void ctrl_FILE_MZ_female(list* list_people) {

	node* nodo_persona = head(list_people);
	list_type dati_identificativi;

	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "TOPO");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "LINA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "03/05/1903");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "GENOVA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "TPOLNI03E43D969Y");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_PTR_NOT_NULL(dati_identificativi);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "PAPE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "RINA");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "18/10/1946");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "TRENTO");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "PPARNI46R58L378Z");

	nodo_persona = next(list_people, nodo_persona);
	dati_identificativi = content(nodo_persona);
	CU_ASSERT_STRING_EQUAL(dati_identificativi->surname, "MINE");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->name, "FUJIKO MARGOT");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_date, "25/08/1980");
	CU_ASSERT_STRING_EQUAL(dati_identificativi->birth_place, "CATANIA");
	CU_ASSERT_EQUAL(dati_identificativi->sex, 'F');
	CU_ASSERT_STRING_EQUAL(dati_identificativi->cf, "MNIFKM80M65C351J");

	CU_ASSERT_PTR_NULL(next(list_people, nodo_persona));
}
