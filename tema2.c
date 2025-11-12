//BARAN Denis-Constantin
// Grupa 312CC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------- arborele multi-way(cai) ---------------------------
typedef struct multi_node {
	int code;
	char *name;
	struct multi_node **children;
	int nr_children;
} multi_node;

//--------------------------- arborele huffman --------------------------------
typedef struct huff_node {
	int prior;
	char code;
	char *name;
	struct huff_node *left, *right;
	struct multi_node *multi;
} huff_node;

//------------------------- min-movila (min-heap) -----------------------------
typedef struct {
	int max_heap_size;
	int size;
	huff_node **elements;
} pr_queue, *a_pr_queue;

//------------------------ coada pentru afisare -------------------------------
typedef struct queue {
	huff_node *elem;
	struct queue *next;
} queue;

// -------------------------------- Cerinta 1 ---------------------------------

// Functia pentru initializarea cozii pentru min-heap (movila) si alocarea
// memoriei necesare
void pq_init(a_pr_queue q, int max_size)
{
	q->size = 0;
	q->max_heap_size = max_size + 1;
	q->elements = calloc((q->max_heap_size), sizeof(huff_node *));
}

// Functia de filtrare in sus a unui nod, se pastreaza prioritatea de min heap
// interschimband nodurile daca este necesar
void sift_up(a_pr_queue q, int i)
{
	while (i > 1 &&
		   (q->elements[i]->prior < q->elements[i / 2]->prior ||
			(q->elements[i]->prior == q->elements[i / 2]->prior &&
			 strcmp(q->elements[i]->name, q->elements[i / 2]->name) < 0))) {
		huff_node *temp = q->elements[i];
		q->elements[i] = q->elements[i / 2];
		q->elements[i / 2] = temp;
		i /= 2;
	}
}

// Functia de inserare a unui nod cu prioritati in coada
// se foloseste sift_up pentru a se mentine proprietatea de min heap ul
void insert_node(a_pr_queue q, huff_node *node)
{
	if (q->size >= q->max_heap_size - 1) {
		printf("Eroare: Heap plin!\n");
		return;
	}
	q->elements[++(q->size)] = node;
	//am adaugat sift_up pentru a pastra proprietatea de min heap
	sift_up(q, q->size);
}

// Functia de filtrare in jos a unui nod interschimba nodurile in cazul in care
// elementul nu este pozitionat corect
void sift_down(a_pr_queue q, int k)
{
	int j;
	while (2 * k <= q->size) {
		j = 2 * k;
		if (j < q->size &&
			(q->elements[j]->prior > q->elements[j + 1]->prior ||
			 (q->elements[j]->prior == q->elements[j + 1]->prior &&
			  strcmp(q->elements[j]->name, q->elements[j + 1]->name) > 0))) {
			j++;
		}
		// ne oprim cand fiul din dreapta este mai mare
		if (q->elements[k]->prior < q->elements[j]->prior ||
			(q->elements[k]->prior == q->elements[j]->prior &&
			 strcmp(q->elements[k]->name, q->elements[j]->name) <= 0)) {
			break;
		}
		//interschimbam elementul curent cu cel mai mare
		huff_node *temp = q->elements[k];
		q->elements[k] = q->elements[j];
		q->elements[j] = temp;
		k = j;
	}
}

// Functia de extragere a minimului, care extrage elementul minim din coada
// si il sterge din coada
huff_node *extract_min(a_pr_queue q)
{
	if (q->size == 0) {
		printf("Heap gol!\n");
		return NULL;
	}
	huff_node *min_element = q->elements[1];
	q->elements[1] = q->elements[q->size];
	q->size--;
	sift_down(q, 1);
	return min_element;
}

//Functia de construire a arborelui Huffman din min-heap
huff_node *build_huffman_tree(a_pr_queue q)
{
	while (q->size > 1) {
		//extragem cele mai mici elemente din min heap
		huff_node *left = extract_min(q);
		huff_node *right = extract_min(q);
		huff_node *parent = calloc(1, sizeof(huff_node));
		//initiliaziam parintele
		parent->left = NULL;
		parent->right = NULL;
		parent->multi = NULL;

		// plasam in stanga satelitul cu frecventa mai mica si in caz de
		// egalitate plasam in partea stang primul satelin in ordine alfabetica
		if (left->prior == right->prior &&
			strcmp(left->name, right->name) > 0) {
			huff_node *tmp = left;
			left = right;
			right = tmp;
		}

		//calculam prioritatea parintelui insumand suma prioritatilor fiilor
		parent->prior = left->prior + right->prior;
		//atribuim in partea stanga codul 0 si in cea dreapta 1
		left->code = '0';
		right->code = '1';

		//concatenam numele fiilor pentru a forma numele parintelui
		parent->name = strdup(left->name);
		int len = strlen(left->name) + strlen(right->name) + 1;
		//strdup aloca memorie si deci realocam memoria pentru a putea
		// concatena si numele fiului drept
		parent->name = realloc(parent->name, len);
		strcat(parent->name, right->name);

		//atribuim cele mai mici noduri parintelui si il inseram in min heap
		parent->left = left;
		parent->right = right;
		insert_node(q, parent);
	}
	return extract_min(q);
}

// functia enqueue adauga un nod intr-o coada
void enqueue(queue **q, huff_node *a)
{
	queue *new_node = calloc(1, sizeof(queue));
	new_node->elem = a;
	new_node->next = NULL;
	if (!*q) {
		*q = new_node;
		return;
	}
	queue *temp = *q;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

//functia dequeue elimina un nod dintr-o coada de noduri
huff_node *dequeue(queue **q)
{
	if (!*q)
		return NULL;
	queue *temp = *q;
	huff_node *node = temp->elem;
	*q = temp->next;
	free(temp);
	return node;
}

//aplicam parcurgerea pe nivele pentru afisarea arborelui Huffman format
void level_order(huff_node *root, FILE *out)
{
	if (!root)
		return;

	queue *q = NULL;
	enqueue(&q, root);

	while (q) {
		int node_count = 0;
		queue *temp = q;
		//calculam cate noduri sunt pe nivelul curent
		while (temp) {
			node_count++;
			temp = temp->next;
		}

		//afisam toate nodurile de pe acest nivel
		for (int i = 0; i < node_count; i++) {
			//scoatam primul nod din coada
			huff_node *aux = dequeue(&q);
			fprintf(out, "%d-%s ", aux->prior, aux->name);

			//aduagam fiul stang si drept, daca exista
			if (aux->left)
				enqueue(&q, aux->left);
			if (aux->right)
				enqueue(&q, aux->right);
		}
		fprintf(out, "\n");
	}
}

// -------------------------------- Cerinta 2 ---------------------------------

// Functia decodifica un sirul codificat "enc" parcurgand arborele binar
// si sterge din sir secventa de caractere care reprezinta codificarea nodului
int decode_huffman(huff_node *root, char *enc, FILE *out)
{
	huff_node *tree = root;
	// parcurgem arborele conform codului din 'enc'
	while (tree->left && tree->right && *enc != '\0') {
		//atat partea stanga, cat si cea dreapta
		if (tree->left && tree->left->code == *enc) {
			tree = tree->left;
			enc++;
		} else if (tree->right && tree->right->code == *enc) {
			tree = tree->right;
			enc++;
		}
	}

	// verificam ca am ajuns la un nod frunza
	// in caz afirmativ, am decodifat o parte din sir, il vom afisa
	if (!tree->left && !tree->right)
		fprintf(out, "%s ", tree->name);
	else
		fprintf(out, "\n");

	//daca nu am ajuns la finalul sirului, continuam decodificarea
	if (*enc != '\0')
		decode_huffman(root, enc, out);

	return 0;
}

// -------------------------------- Cerinta 3 ---------------------------------

// Functia care codifica un sir bazat pe numele unui nod, cautam nodul in
// arborele binar Huffman si transmitem sirul encode_str cu dublu pointer
// pentru a se modifica
int find_encode(huff_node *root, char *name, char **encode_str, int *cap,
				int *size)
{
	if (!root)
		return 0;

	//daca am ajuns la o furnza inseamna ca am gasit codificarea
	if (!root->left && !root->right) {
		if (strcmp(root->name, name) == 0)
			return 1;
		return 0;
	}

	//daca marimea este mai mare decat capacitatea alocata, realocam memoria
	// pentru sirul codificat
	if (*size + 1 >= *cap) {
		*cap = *cap * 2;
		*encode_str = realloc(*encode_str, *cap);
	}

	//cautam in partea dreapta a arborelui
	if (root->left) {
		//punem codul nodului in sirul nostru pentru a afla codificarea sa
		(*encode_str)[*size] = root->left->code;
		(*size)++;
		//continuam iterarea prin arbore
		if (find_encode(root->left, name, encode_str, cap, size))
			return 1;
		(*size)--;
	}
	//cautam codificarea si in partea dreapta
	if (root->right) {
		//analog punem codificarea in sirul nostru si marim lungimea
		(*encode_str)[*size] = root->right->code;
		(*size)++;
		if (find_encode(root->right, name, encode_str, cap, size))
			return 1;
		(*size)--;
	}
	return 0;
}

//functia afiseaza codificarea unui nod din arborele Huffman
void encode_huffman(huff_node *root, char *name, FILE *out)
{
	huff_node *tree = root;
	char *encode_str = calloc(strlen(name), 1);
	int size = 0, cap = strlen(name);

	find_encode(tree, name, &encode_str, &cap, &size);
	encode_str[size] = '\0';
	fprintf(out, "%s", encode_str);
	free(encode_str);
}

// -------------------------------- Cerinta 4 ---------------------------------

// Functia care gaseste cel mai apropiat parintele a doua noduri cu numele
// date, name1 si name2 (lowest common ancestor)
huff_node *find_lca(huff_node *tree, char *name1, char *name2)
{
	if (!tree)
		return tree;

	// daca numele unui nod se afla in partea stang si celalte in partea dreapta
	// inseamna ca am gasit lca-ul nodurilor si il returnam
	if (strstr(tree->left->name, name1) && strstr(tree->right->name, name2))
		return tree;
	else if (strstr(tree->left->name, name2) &&
			 strstr(tree->right->name, name1))
		return tree;

	// daca numele nodului se afla in partea stang continuam sa cautam pe acea
	// ramura
	if (tree->left && strstr(tree->left->name, name1) &&
		strstr(tree->left->name, name2))
		return find_lca(tree->left, name1, name2);

	// cautam si in ramura dreapta a arborelui in cazul in care numele este
	// continut
	if (tree->right && strstr(tree->right->name, name2) &&
		strstr(tree->right->name, name2))
		return find_lca(tree->right, name1, name2);

	// in cazul in care nu s-a gasit lca-ul nodurilor returnam NULL
	// cazul nu apare in testele noastre, deocarece se dau mereu noduri valide
	// dar punem acest return pentru rabustete si a nu aparea warning uri
	return NULL;
}

// -------------------------------- Cerinta 5 ---------------------------------

//Functia gaseste un nod in arborele huffman si il returneaza
huff_node *find_node(huff_node *root, char *name)
{
	if (!root)
		return NULL;

	//daca nodul contine numele, inseamna ca l-am gasit si il returnam
	if (!strcmp(root->name, name))
		return root;

	//cautam in partea stang si in partea dreapta numele cautat
	if (root->left && strstr(root->left->name, name))
		return find_node(root->left, name);
	if (root->right && strstr(root->right->name, name))
		return find_node(root->right, name);

	//analog pentru robustete returnam NULL, caz imposibil in tema noastra
	//deoarece mereu vom gasim nodul
	return NULL;
}

// Functia de initializare a unui arbore multitree/multicai
multi_node *init_multi_node(multi_node *tree)
{
	tree = calloc(1, sizeof(multi_node));
	tree->nr_children = 0;
	//inlocuim prioritatea cu codul specific
	tree->code = 0;
	tree->name = calloc(16, 1);
	tree->children = NULL;
	return tree;
}

// Functia de inserare a unul copil in vectorul parintelui avand datele
// specificate, cod si nume
void multi_insert(multi_node *tree, int code, char *name)
{
	//Initalizam datele nodului
	multi_node *node = calloc(1, sizeof(multi_node));
	node->code = code;
	node->name = strdup(name);
	node->nr_children = 0;
	node->children = NULL;

	int i;
	//Il adaugam noul nod in lista de copii a parintelui pe pozitia libera
	for (i = 0; i < tree->nr_children; i++) {
		if (!tree->children[i]) {
			tree->children[i] = node;
			return;
		}
	}
}

//Functia de gasire a unui nod din arborele mutli-way/multicai
multi_node *find_multi_node(multi_node *root, char *name)
{
	if (!root)
		return NULL;

	//Daca am gasit nodul il returnam
	if (strcmp(root->name, name) == 0)
		return root;

	//continuam sa cautam nodul in lista de copii ai sai
	int i;
	for (i = 0; i < root->nr_children; i++) {
		if (root->children[i]) {
			multi_node *found = find_multi_node(root->children[i], name);
			//daca nodul este gasit (nu este NULL), continuam cautarea
			if (found)
				return found;
		}
	}
	return NULL;
}

// Functia de gasire a distantei de la radacina la nodul respectiva
int find_depth_multi_tree(multi_node *root, char *name)
{
	if (!root)
		return -1;

	//daca nodul a fost gasit, returnam zero pentru acumularea lungimii
	if (strcmp(root->name, name) == 0)
		return 0;

	int d;
	//cautam nodul in lista de copii ai nodului
	for (int i = 0; i < root->nr_children; i++) {
		if (root->children[i]) {
			d = find_depth_multi_tree(root->children[i], name);
			// daca s-a gasit nodul respectiv, aculam lungimea sa si cautam
			// nodul in adancime
			if (d != -1)
				return d + 1;
		}
	}

	return -1;
}

// Functia de gasire a unui nod in arborele binar extins cu arbori multi-way
int find_depth_tree(huff_node *root, char *name)
{
	if (!root)
		return -1;

	//daca am gasit nodul returnam zero pentru acumularea lungimii
	if (strcmp(root->name, name) == 0)
		return 0;

	int d;
	//cautam nodul in partea stanga a arborelui
	d = find_depth_tree(root->left, name);
	if (d != -1)
		//daca s-a gasit acumulam distanta
		return d + 1;

	//continuam cautarea si in partea dreapta a arborelui
	d = find_depth_tree(root->right, name);
	if (d != -1)
		return d + 1;

	//cautam daca are atasat un arbore multicai si in acel arbore folosind
	// o functia specifica
	if (root->multi) {
		d = find_depth_multi_tree(root->multi, name);
		if (d != -1)
			return d + 1;
	}

	// returnam -1 in cazul in care nu s-a gasit nodul
	// de data aceasta, returnarea cazului null este necesara
	return -1;
}

// Functia care gaseste catre nodul unui arbore multi-way, salvand nodurile in
// vectorul de noduri mpath care se transmite cu  "**" (dublu pointer) pentru
// a se modifica
int get_path_multi(multi_node *root, char *name, multi_node **mpath,
				   int *mpath_len)
{
	if (!root)
		return 0;

	//adaugam nodul curent la path ul nostru
	mpath[(*mpath_len)++] = root;

	//daca am gasit nodul ne oprim
	if (strcmp(root->name, name) == 0)
		return 1;

	// in caz contrar cautam in vectorul de copii
	for (int i = 0; i < root->nr_children; i++) {
		if (get_path_multi(root->children[i], name, mpath, mpath_len))
			return 1;
	}

	// daca nu am gasit nodul in aceasta ramura, scadem lungimea path-lui si
	// returnam zero
	(*mpath_len)--;
	return 0;
}

//Functia pentru calculul adancimii maxime pentru un arbore multi-way
int max_depth_multi_tree(multi_node *root)
{
	if (!root)
		return 0;

	int max_child = 0;
	int i;
	//caltam maximul lungimii de la radacina arborelui mutiway la un nod frunza
	for (i = 0; i < root->nr_children; i++) {
		int d = max_depth_multi_tree(root->children[i]);
		if (d > max_child)
			max_child = d;
	}
	return max_child + 1;
}

// Functia pentru calculul unui arbore binar extins
int max_depth_tree(huff_node *root)
{
	if (!root)
		return 0;

	//cautam lungimea cea mai lunga in arborele din stanga si dreapta
	int d_left = max_depth_tree(root->left);
	int d_right = max_depth_tree(root->right);

	//cautam si in arborele multi-way
	int d_multi = 0;
	if (root->multi)
		d_multi = max_depth_multi_tree(root->multi) + 1;

	int d;
	if (d_left > d_right)
		d = d_left;
	else
		d = d_right;

	if (d_multi > d)
		d = d_multi;

	//returnam d+1 pentru arborele curent
	return d + 1;
}

//Functia de extragere a path-ului de la radacina la un nod, care salveaza
// nodurile in vectorul path
int get_path_huff(huff_node *root, char *name, huff_node **path, int *path_len)
{
	if (!root)
		return 0;

	//adagam vectorul curent la path si crestem lungimea
	path[(*path_len)++] = root;

	//ne oprim daca am gasit nodul respectiv
	if (strcmp(root->name, name) == 0)
		return 1;

	//cautam vectorul in partea stanga a arborelui
	if (get_path_huff(root->left, name, path, path_len))
		return 1;

	//analog cautam si in partea dreapta
	if (get_path_huff(root->right, name, path, path_len))
		return 1;

	//daca nodul are atasat arbore multiway continuam cautarea si acolo
	if (root->multi) {
		int multi_alloc = max_depth_multi_tree(root->multi);
		if (multi_alloc > 0) {
			multi_node **mpath = calloc(multi_alloc, sizeof(multi_node *));
			int mpath_len = 0;

			if (get_path_multi(root->multi, name, mpath, &mpath_len)) {
				//adaugam nodurile multi_way in path
				for (int i = 0; i < mpath_len; i++) {
					path[(*path_len)++] = (huff_node *)mpath[i];
				}
				free(mpath);
				return 1;
			}
			free(mpath);
		}
	}

	//daca nu am gasit nodul curent, reducem lungimea si returnam 0
	(*path_len)--;
	return 0;
}

int get_lca_depth(huff_node *root, char *name1, char *name2)
{
	//calculam adancimea maxima pentru a alocare
	int max_depth = max_depth_tree(root);

	// alocam vectorii path cu lungimea maxima
	huff_node **path1 = calloc(max_depth, sizeof(huff_node *));
	huff_node **path2 = calloc(max_depth, sizeof(huff_node *));

	int local_len1 = 0, local_len2 = 0;
	//extragem caile folosind vectorii alocati
	get_path_huff(root, name1, path1, &local_len1);
	get_path_huff(root, name2, path2, &local_len2);

	//comparam rand pe rand nodurile din path si cand gasim o diferenta
	// returnam lungimea gasita
	int i = 0;
	while (i < local_len1 && i < local_len2 && path1[i] == path2[i])
		i++;

	free(path1);
	free(path2);

	return i - 1;
}

//--------------------------------- free --------------------------------------

//Functia de eliberare arborele multi-way
void free_multi_tree(multi_node *root)
{
	if (!root)
		return;

	for (int i = 0; i < root->nr_children; i++)
		if (root->children[i])
			free_multi_tree(root->children[i]);

	free(root->name);
	free(root->children);
	free(root);
}

//Functia de eliberare pentru arborele huffman
void free_huffman_tree(huff_node *root)
{
	if (!root)
		return;

	free_huffman_tree(root->left);
	free_huffman_tree(root->right);

	if (root->multi)
		free_multi_tree(root->multi);

	free(root->name);
	free(root);
}

//Functia de eliberare pentru min-heap
void free_heap(a_pr_queue q)
{
	for (int i = 1; i <= q->size; i++) {
		free(q->elements[i]->name);
		free(q->elements[i]);
	}
	free(q->elements);
	free(q);
}

void free_queue(queue *q)
{
	while (q) {
		queue *temp = q;
		q = q->next;
		free(temp);
	}
}

//--------------------------------- Main --------------------------------------

int main(int argc, char **argv)
{
	//folosim argumentele pentru a evita warning-uri
	(void)argc;
	//deschidem fisierele necesare
	FILE *in = fopen(argv[2], "r");
	FILE *out = fopen(argv[3], "w");

	//vom citi numarul de elemente din min-heap
	int n, i;
	fscanf(in, "%d", &n);

	//aloca memorie pentru min-heap si initializam coada de prioritati
	a_pr_queue q = calloc(1, sizeof(pr_queue));
	pq_init(q, n);

	char *name = calloc(16, 1);
	for (i = 0; i < n; i++) {
		//alocam memorie pentru arborele huffman
		huff_node *node = calloc(1, sizeof(huff_node));
		//citim priritatea
		fscanf(in, "%d", &node->prior);

		//citim numele si il atribuim
		fgetc(in);
		fgets(name, 16, in);

		if (name[strlen(name) - 1] == '\n')
			name[strlen(name) - 1] = '\0';
		node->name = strdup(name);
		node->left =  NULL;
		node->right = NULL;
		node->multi = NULL;

		//il inseram in coada
		insert_node(q, node);
	}
	//contruim arborele huffman folosim min-heap ul creat
	huff_node *root = build_huffman_tree(q);

	if (strstr(argv[1], "-c1")) {
		//afisam pe lungimi arobele
		level_order(root, out);
	} else if (strstr(argv[1], "-c2")) {

		//alocam o lungime maxima a codificarii
		name = realloc(name, 1000);
		//citim numarul de codificari
		fscanf(in, "%d", &n);
		fgetc(in);

		//citim rand pe rand codificarile si folosim functia pentru a afisa
		//ce avem nevoie
		for (i = 0; i < n; i++) {
			fgets(name, 1000, in);
			//eliminam newline, pentru ca stim ca fgets citeste si pana la '\n'
			name[strlen(name) - 1] = '\0';

			decode_huffman(root, name, out);
			fprintf(out, "\n");
		}
	} else if (strstr(argv[1], "-c3")) {
		//citim numarul de noduri
		fscanf(in, "%d", &n);

		//alocam dinamic lungimea maxima a unui nod
		name = realloc(name, 1000);
		fgetc(in);
		for (i = 0; i < n; i++) {
			fgets(name, 1000, in);
			name[strlen(name) - 1] = '\0';

			//folosim functia de codificare pentru nod
			encode_huffman(root, name, out);
		}

	} else if (strstr(argv[1], "-c4")) {
		fscanf(in, "%d", &n);
		name = realloc(name, 1000);
		fgetc(in);

		//citim numele unui nod
		fgets(name, 1000, in);
		name[strlen(name) - 1] = '\0';
		char *name2 = calloc(1000, 1);

		//citim si al doilea nod
		fgets(name2, 1000, in);
		name2[strlen(name2) - 1] = '\0';
		huff_node *lca = find_lca(root, name, name2);

		//luam nodurile doua cate doua si gasim cel mai apropiat parintele
		for (i = 0; i < n - 1; i++) {
			fgets(name2, 1000, in);
			name2[strlen(name2) - 1] = '\0';

			//daca nodul nu este inclus in nume inseamna ca lca-ul trebuie
			//extins
			if (!strstr(lca->name, name2))
				lca = find_lca(root, lca->name, name2);
		}
		fprintf(out, "%s\n", lca->name);
		free(name2);
	} else {
		//citim numarul de arbori multiway atasati
		int nr_multi_trees;
		fscanf(in, "%d", &nr_multi_trees);
		fgetc(in);

		//atasam fiecare arbore la nodul citit
		for (i = 0; i < nr_multi_trees; i++) {
			char *bin_name = calloc(16, 1);

			//citim nodul caruia i se ataseaza arborele
			fgets(bin_name, 16, in);
			bin_name[strlen(bin_name) - 1] = '\0';

			//citim arborele care trebuie atasat
			multi_node *multi_tree = init_multi_node(NULL);
			fscanf(in, "%d", &multi_tree->code);
			fgetc(in);
			fgets(multi_tree->name, 16, in);
			multi_tree->name[strlen(multi_tree->name) - 1] = '\0';

			//gasim nodul caruia i se ataseaza arborele multiway
			huff_node *bin_node = find_node(root, bin_name);

			// se creeza lagatura intre arborele binar si cel multiway
			bin_node->multi = multi_tree;

			//citim numarul de parinti atasati
			int nr_parents;
			fscanf(in, "%d", &nr_parents);
			fgetc(in);

			int j;
			char *m_name = calloc(16, 1);
			for (j = 0; j < nr_parents; j++) {

				//citim numele de la tastatura
				fgets(m_name, 16, in);
				if (strlen(m_name) > 0 && m_name[strlen(m_name) - 1] == '\n')
					m_name[strlen(m_name) - 1] = '\0';

				//alocam memorie pentru parinte
				multi_node *parent = find_multi_node(multi_tree, m_name);

				//citim numarul de copii
				fscanf(in, "%d", &parent->nr_children);
				parent->children =
					calloc(parent->nr_children, sizeof(multi_node *));

				//citim credidentialele fiecarui copil
				int k, code;
				for (k = 0; k < parent->nr_children; k++) {
					fscanf(in, "%d", &code);
					fgetc(in);

					fgets(m_name, 16, in);
					m_name[strlen(m_name) - 1] = '\0';
					multi_insert(parent, code, m_name);
				}
			}
			//eliberam string urile folosite
			free(m_name);
			free(bin_name);
		}

		//alocam memorie pentru 2 noduri
		char *name_node1 = calloc(16, 1);
		char *name_node2 = calloc(16, 1);
		fscanf(in, "%s", name_node1);
		fscanf(in, "%s", name_node2);

		int depth1 = find_depth_tree(root, name_node1);
		int depth2 = find_depth_tree(root, name_node2);

		int depth_lca = get_lca_depth(root, name_node1, name_node2);
		int distance = depth1 + depth2 - 2 * depth_lca;
		fprintf(out, "%d\n", distance);

		free(name_node1);
		free(name_node2);
	}

	//elibaram variabila si structurile folosite
	free(name);
	free_huffman_tree(root);
	free_heap(q);

	fclose(in);
	fclose(out);
	return 0;
}
