BARAN Denis-Constantin
Grupa 312CC
Tema 2 SDA

Cerinta 1
Pentru implementarea structurii de Min-Heap (Min-Movila) am folosit structura
prezentata in cursul Domnului Serban Radu de la seria CC, despre Movile. Am
adaugat in plus campul nume si code pe langa campul prioritatii, asa cum se
mentioneaza in cerinta. (campul code este folosit la taskurile 2 si 3)

Se implementeaza asadar o coada de prioritati cu functia pq_init pentru
initializarea si alocarea ei. Se citeste de la tastatura dimensiunea fixa,
alocandu-se cat este necesar (numarul n). Functia sift_up filtreaza un nod in
sus in coada de prioritati in functie de prioritate si in caz de egalitate in
ordine alfabetica, aspect specificat in enunt.
Functia insert_node insereaza in coada de prioritati un nou nod si il foloseste
sift_up pentru a pastra ordinea corecta.
Cazul "Heap plin" este implementat in curs si a fost pastrat pentru robustete,
in schimb nu va fi niciodata apelat deoarece am alocat suficent spatiu la
citirea nodurilor.
Functia sift_down filtreaza in jos un nod in coada de prioritati, asa cum este
implementat si un curs, este implementat in cazul stergerii radacinii cozii de
prioritati
Functia extract_min returneaza cel mai mic nod din coada cu prioritatea cea mai
mica (prioritatea de min-heap), si il sterge folosindu-se de sift_down

Apoi, in enunt ni se cere sa construim un arbore binar luand nodurile doua cate
doua in functie de prioritatea cea mai mica. Mi-am dat seama ca se poate folosi
structura unui arbore Huffman care se bazeaza exact pe cerinta temei,
construirea unui arbore binar dintr-un min-heap. Unicul lucru adaugat este
concatenarea numelor, aspect solicitat in cerinta. Folosim astfel functiile
insert_node si extract_min pentru a extrage nodurile cele mai mici, a crea un
nod parinte cu suma prioritailor si numele obtinut prin concatenarea fiilor sai
Campul code este completat cu 0 pentru fiul din dreapta si 1 pentru fiul din
stanga, motivul implementarii este precizat in Cerinta 2
Nodul se insereaza in coada de prioritati si in locul fiilor sai, si se
returneaza arborele huffman rezultat in urma algoritmului (ultimul nod din
coada de prioritati reprezentand radacina). Aceasta a fost bazat pe
implementarea din curs si laborator, si de asemenea pe site-ul Geeks for geeks
https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/

Acum ca avem arborele format, trebuie sa il afisam pe niveluri. Am folosit si
aici implementarea din curs care foloseste 2 cozi. Am implementat functia
enqueue de inserare in coada si dequeue de stergere in coada, asa cum au fost
realizate in curs. Am creat functia level order care parcurge pe nivele
arborele binar Huffman si foloseste operatiile enqueue si dequeue pentru a pune
nodurile fii/copii stang si drept in coada apelandu-se cand este necesar pentru
a putea procesa urmatorul nivel. Se introduce newline '\n' dupa fiecare nivel.

Cerinta 2
Pentru aceasta cerinta am adaugat un nou camp in structura campul code. Aceasta
se completeaza odata cu constuctia arborelui. Din construirea nodului parinte,
nodul din stanga se implementeaza cu zero si cel din dreapta cu 1. Daca
parintele devine copilul altui nod, i se atribuie codul necesar. Radacina
ramane singurul nod cu campul "code" gol. Acest camp usureaza decodificarea de
la Cerinta 2

Functia decode_huffman parcurge un arbore huffman in functie de sirul de
caractere enc. Se parcurge nodurile care au codul egal cu caracterul curent din
enc. Se creste codul enc cu cat parcurgem mai mult arborele in adancime. Daca
ajungem la un nod frunza afisam numele acelui nod deoarece inseamna ca o parte
din codul enc a reprezentat codificarea nodului curent.

Daca enc nu a ajuns null ('\0') inseamna ca nu am terminat de codificat si mai
sunt noduri de parcurs asa ca vom apela recursiv functia pana nu vor mai fi
caractere valide (diferite de null) in sirul enc

In main pentru citire alocam numarul maxim de noduri mentionat in cerinta
adica 1000

Cerinta 3
Folosim din nou campul "code" din structura pentru a codifica un nod.
Functia find_encode cauta recursiv un nod in arbore dupa nume si se
opreste cand il gaseste. Intre timp, se salveaza in sirul encode caracterele
gasite in campul "code" al parintilor intalniti pana la gasirea nodului
respectiv. Sirul este transmis cu dublu pointer "**" pentru a se modifica si
a-l putea realoca corespunzator. Daca find_encode returneaza 0 inseamna ca
nodul nu este pe ramura respectiva si trebuie sa scadem dimensiunea

Functia encode_huffman aloca memorie pentru codificarea unui sir si o anumita
capacitate. Aceasta capacitate se mareste in functie de dimensiunea sirului

In main, la citirea fiecarui nod, vom apela functia de codificare a lui astfel
incat sa obtinem codul

Cerinta 4
Functia find_lca gaseste cel mai apropiat parinte a doua noduri. Algoritmul
compara numele din stanga si numele din dreapta a unui nod. Daca cele doua
nume se gasesc in extremitati diferite (stanga si dreapta) atunci inseamna ca
acel nod este parintele (lucru rezultat din contruirea arborelui prin
concatenare). Altfel cauta recursiv in nodurile in partea stanga sau in cea
dreapta a arborelui.
In main se citest nodurile si gasim cel mai apropiat parinte luand nodurile
doua cate doua. Nu cautam din nou lca-ul daca numele nodului citit este deja
continut (lucru care inseamna ca acel nod se afla in subarborele parintelui
respectiv)

Cerinta 5
Pentru aceasta functie am adaugat un camp nou in structura unui nod. Acesta
reprezinta legatura dintre arborele binar si arborele multi-tree

Functia find_node gaseste un nod in arborele binar si il returneaza.
Functioneaza pe acelasi principiu ca la celelate, cautam in partea stanga si in
partea dreapta pana gasim nodul cu numele respectiv

Functia init_multi_node intializeaza un subarbore extins ar arborelui binar
Huffman

Functia multi_insert adauga un copil in vectorul de copii al parintelui din
arborele multiway

Pentru structurile de albori multiway/multicai am revizuit aceste site-uri
https://users.utcluj.ro/~cameliav/fa/Tema7_arbori_multicai.pdf
https://staff.cs.upt.ro/~ioana/sdaa/sdaa/l5/paa5.html

Functia find_multi_node gaseste un nod intr-un arbore binar atasat multi-way
si il returneaza. Acum in loc sa cautam in partea stanga si cea dreapta, vom
cauta in intregul vector de copii al fiecarui nod.

Functia find_depth_multi_tree functioneaza pe acelasi principiu al functiei
anteriore dar in schimb in loc sa returneze lungimea, va returna inaltimea
(adancimea) de la radacina la nodul respectiv. Vom creste cu unu de fiecare
data cand ne adancim in arbore (crestem in nivel) si returnam 0 cand gasim
pentru a putea cumula distanta de la radacina la nod.

Functia find_depth_tree functioneaza pe acelasi principiu doar ca aceasta va
cauta in intregul arbore binar extins. Cautam in stanga, in drepta si daca e
cazul un arborele multi-way atasat. Folosim functia anterioara pentru a cauta
in structura specifica.

Functia get_path_multi gaseste un nod intr-un arbore multi_way un nod cu numele
specificat si retine in vectorul de structuri "mpath" fiecare nod intalnit
(fiecare parinte) pana la gasirea nodului cautat.

Functia max_depth_multi_tree gaseste inaltimea maxima intr-un arbore multi-way,
folosind acelasi principiu ca la functia find_multi_tree. Aceasta este
folositor pentru a aloca o dimensiune maxima a vectorului mpath

Functia max_depth_tree determina analog cea mai mare adancime a arborelui binar
Huffman. Este utila analog pentru a aloca o dimensiune maxima a vectorului
path

Functia get_path_huff determina drumul de la radacina la un nod cautat. Acesta
retine analog nodurile intalinte in vectorul mpath. Daca este nevoie sa cautam
in arborele multiway vom aloca memorie pentru drumul in arbore multi-way si
folosim functia specifica. Vom pune in vectorul nostru de noduri "path"
nodurile obtinute pentru a le verifica ulterior.

Functia get_lca_dept gaseste lungimea celui mai apropiat stramos in arborele
extins Huffman. Aloca memorie pentru 2 drumuri (path1 si path2) si foloseste
functia auxiliara pentru a obtine drumurile catre noduri. Compara nodurile
comune, rand pe rand pana cand intalneste un nod diferit. Se returneaza
lungimea respectiva la primul nod distinct intalnit

Functia free_multi_tree elibereaza memoria pentru un subarbore multi_tree din
arborele extins binar Huffman

Functia free_huffman_tree elibereaza memoria pentru nodurile arborelui huffman,
stang si dreapta, si daca e cazul si pentru subarborele atasat folosind functia
anterioara

Functia free_heap elibereaza memoria pentru min-heap
Functia free_queue elibereaza memoria pentru coada, aceasta este apelata de
functia de afisare pe nivele

Main

Functia main dechide fisierele necesare
"(void)argc;" este folosit pentru a elimina warning ul produs de makefile

Vom citi numarul de noduri/sateliti n, din fisierul de intrare si apoi n noduri
pe care le inseram in coada de prioritati. Folosim apoi build_huffman_tree
pentru construi arborelel binar

Pentru cerinta 1 se apeleaza functia de afisare pe nivele

Pentru cerinta 2 vom citi n codificari si vom folosi functia decode_huffman
pentru a le decodifica

Pentru cerinte 3 citim n noduri si folosim functia encode_huffman pentru a o
apela

Pentru cerinta 4 citim primele 2 noduri de la tastatura. Calculam cel mai
apropiat parinte folosind functia find_lca. Folosind acest nod lca, il folosim
pentru a gasi si ceilalti parinti. Daca in schimb numele citit este continut de
lca, inseamna ca nodul se afla deja in subarborele parintelui si nu mai este
nevoie sa il apelam. Altfel calculam lca-ul dintre vechiul lca si nodul curent.
La final ar trebui sa avem lca-ul tuturor nodurilor cititie si il afisam

Pentru cerinta 5, citim numarul de subarbori.
Folosim functia find_node sa stim cui ii vom atasa arborele multi-tree/multicai
Citim apoi numarul de parinti pe care ii va contine subarborele, numele lor si
apoi vectorul sau de copii si ne folosim de functiile find_multi_node si
multi_insert pentru atasarea corespunzatoare.
Dupa citirea tututor subarborilor multi-way, vom calcula distanta de la
radacina la cele doua noduri date. Apoi folosim functia get_lca_depth pentru a
calcula distanta de la radacina la cel mai apropiat parinte al nodurile
respective. Vom gasi distanta cerita folosind formula
dist1 + dist2 - 2* dist_lca, formula si parte din implementari gasite pe
site-ul greeks for geeks:
https://www.geeksforgeeks.org/find-distance-between-two-nodes-of-a-binary-tree/
Pentru calculul distantei am revizuit si aceast thread
https://codeforces.com/blog/entry/20159

Nu uitam sa inchidem fisierele si sa eliberam structurile