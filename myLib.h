#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <Windows.h>
#include <time.h>
#define N 15
#define STRING 40

typedef struct {
	int id;
	int popularity;
	char name[STRING];
	float priceBuy;
	float priceSell;
	int quantity;
	float weight;
	char type[STRING];//тип кави (мелена,зерно...)
	char country[STRING];
	char composition[STRING];//склад кави (арабіка,робуста...)
} Product;

typedef struct {
	Product product;
	float priceAll;
	int quantityCart;
} Cart;

//Глобальні змінні

Product* massiveProd = NULL;
Cart* massiveCart = NULL;
int cassa = 0;
int cart = 0;
int empty = 0;
float totalPrice = 0.0;
int num = 0;
int id = 0;

//Функція для ініціалізації товару

Product initProduct(int id) {
	Product Product;
	puts("Ââåä³òü ³íôîðìàö³þ ïðî òîâàð\n");
	Product.id = id;
	Product.popularity = 0;

	printf("Ââåä³òü íàçâó:");
	scanf_s("%s", Product.name, STRING);

	do {
		printf("Ââåä³òü ö³íó çàêóïêè:");
		scanf_s("%f", &Product.priceBuy);
	} while (Product.priceBuy <= 0.0);

	do {
		printf("Ââåä³òü ö³íó ïðîäàæó:");
		scanf_s("%f", &Product.priceSell);
	} while (Product.priceSell <= Product.priceBuy);

	do {
		printf("Ââåä³òü ê³ëüê³ñòü òîâàðó:");
		scanf_s("%d", &Product.quantity);
	} while (Product.quantity <= 0);

	do {
		printf("Ââåä³òü ìàñó íåòòî â ãðàìàõ:");
		scanf_s("%f", &Product.weight);
	} while (Product.weight <= 0);

	printf("Ââåä³òü òèï êàâè (ìåëåíà,çåðíî...):");
	scanf_s("%s", Product.type, STRING);

	printf("Ââåä³òü êðà¿íó âèðîáíèêà:");
	scanf_s("%s", Product.country, STRING);

	printf("Ââåä³òü ñêëàä êàâè (àðàá³êà,ðîáóñòà...):");
	scanf_s("%s", Product.composition, STRING);

	return Product;
}

//Функція для ініціалізації кошика

Cart initCart(Product Product, int quantity) {
	Cart Cart;
	Cart.quantityCart = quantity;
	Cart.priceAll = quantity * Product.priceSell;
	Cart.product = Product;
	return Cart;
}

//Функції виведення інформації

void showProduct(Product Product)
{
	printf("\n%d) %s  %s  %s  %s | âàãà %.2f ãð.|\nçàê. %.2f ãðí. | ïðîä. %.2f ãðí. | ê³ëüê. %d | ïîï. %d |\n", Product.id, Product.name,
		Product.type, Product.composition, Product.country, Product.weight, Product.priceBuy, Product.priceSell, Product.quantity, Product.popularity);
}

void showArr(Product* massive)
{
	if (num <= 0) {
		printf("Ñïèñîê ïóñòèé!\n");
		return;
	}
	puts("\nid) Íàçâà òîâàðó  òèï êàâè  ñêëàä êàâè  êðà¿íà âèðîáíèêà | ìàñà íåòòî |\n ö³íà çàêóïêè | ö³íà ïðîäàæó | ê³ëüê³ñòü òîâàðó | ïîïóëÿðí³ñòü |");

	for (int i = 0; i < num; i++)
	{
		showProduct(massive[i]);
	}
}

void showUserProduct(Product Product)
{
	printf(")Êàâà %s %s %.2f ãð. - %.2f ãðí.\n%s %s íà ñêëàä³: %d øò.\n", Product.name,
		Product.type, Product.weight, Product.priceSell, Product.composition, Product.country, Product.quantity);
}

void showUserArr()
{
	if (num <= 0 || num - empty == 0) {
		printf("Ñïèñîê ïóñòèé!\n");
		return;
	}

	for (int i = 0; i < num - empty; i++)
	{
		printf("\n%d", i + 1);
		showUserProduct(massiveProd[i]);
	}
}

void showCart(Cart Cart)
{
	printf(")Êàâà %s %s %.2f ãð. - %døò. x %.2f ãðí.- %.2f ãðí.\n%s %s\n", Cart.product.name,
		Cart.product.type, Cart.product.weight, Cart.quantityCart, Cart.product.priceSell, Cart.priceAll, Cart.product.composition, Cart.product.country);
}

void showCartArr(Cart* massive, int num, int mode)
{
	if (num <= 0 && mode) {
		printf("Êîøèê ïîðîæí³é,ïîïîâí³òü éîãî.\n");
		return;
	}

	for (int i = 0; i < num; i++)
	{
		printf("\n%d", i + 1);
		showCart(massive[i]);
	}
	if (mode) {
		printf("\n________________________ Çàãàëüíà ö³íà - %.2f ãðí.\n", totalPrice);
	}
}

void showEdit(Product Product)
{
	printf("\n1)Íàçâà: %s\n2)Òèï: %s\n3)Ñêëàä: %s\n4)Êðà¿íà: %s\n5)Âàãà: %.2f\n6)Ö³íà çàêóïêè: %.2f\n7)Ö³íà ïðîäàæó: %.2f\n8)Ê³ëüê³ñòü: %d\n", Product.name,
		Product.type, Product.composition, Product.country, Product.weight, Product.priceBuy, Product.priceSell, Product.quantity);
}

//Фунції запису у файл

void writeFile(Product* massive) {
	FILE* file;
	fopen_s(&file, "Products.txt", "w");
	if (file == NULL) {
		puts("Failed to open file \n");
		exit(1);
	}
	fwrite(&num, sizeof(num), 1, file);
	fwrite(&id, sizeof(id), 1, file);
	for (int i = 0; i < num; i++) {
		fwrite(&massive[i], sizeof(massive[i]), 1, file);
	}
	fclose(file);
}

FILE* openFile() {
	FILE* file;
	fopen_s(&file, "searchHistory.txt", "a");
	if (file == NULL) {
		puts("Failed to open file \n");
		exit(1);
	}
	return file;
}

void searchMessage(int mode, char* message, char* search) {
	if (!mode) {
		FILE* file = openFile();
		fprintf(file, message, search);
		fclose(file);
	}
}

void searchMessageF(int mode, char* message, float start, float finish) {
	if (!mode) {
		FILE* file = openFile();
		fprintf(file, message, start, finish);
		fclose(file);
	}
}

void noSearch(int found, int mode, char* message, char* search) {
	if (!found) {
		printf(message, search);
		if (!mode) {
			FILE* file = openFile();
			fprintf(file, "No search results\n");
			fclose(file);
		}
	}
}

void noSearchF(int found, int mode, char* message, float start, float finish) {
	if (!found) {
		printf(message, start, finish);
		if (!mode) {
			FILE* file = openFile();
			fprintf(file, "No search results\n");
			fclose(file);
		}
	}
}
void writeHistory(Product Product) {
	FILE* file = openFile();
	fprintf(file, "%d) %s %s %s weight - %.2f price - %.2f\n", Product.id, Product.name, Product.type, Product.composition, Product.weight, Product.priceSell);
	fclose(file);
}
void writeDel(Product Product) {
	FILE* file;
	fopen_s(&file, "Deleted.txt", "a");
	if (file == NULL) {
		puts("Failed to open file \n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0) {
		fprintf(file, "id) Íàçâà òîâàðó  òèï êàâè  ñêëàä êàâè  êðà¿íà âèðîáíèêà | ìàñà íåòòî |\n ö³íà çàêóïêè | ö³íà ïðîäàæó | ê³ëüê³ñòü òîâàðó | ïîïóëÿðí³ñòü |\n");
	}
	fprintf(file, "\n%d) %s  %s  %s  %s | weight %.2f gram|\nbuy %.2f UAH | sell %.2f UAH | quantity %d | popylarity %d |\n", Product.id, Product.name,
		Product.type, Product.composition, Product.country, Product.weight, Product.priceBuy, Product.priceSell, Product.quantity, Product.popularity);
	fclose(file);
}
void writeSecurity(int mode, char* pass) {
	char status[N];
	if (mode)
		strcpy_s(status, sizeof(status), "successfully");
	else
		strcpy_s(status, sizeof(status), "not successful");

	FILE* file;
	fopen_s(&file, "Security.txt", "a");
	if (file == NULL) {
		puts("Failed to open file \n");
		exit(1);
	}
	time_t mytime = time(NULL);
	struct tm* now = localtime(&mytime);
	fprintf(file, "Entry watering, used password: %s ,status: %s\nDate: %d.%d.%d\nTime: %d:%d:%d\n\n", pass, status, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
	fclose(file);
}
void writeCassa() {
	FILE* file;
	fopen_s(&file, "Cassa.txt", "a");
	if (file == NULL) {
		printf("Failed to open file \n");
		exit(1);
	}
	for (int i = 0; i < cart; i++) {
		fwrite(&massiveCart[i], sizeof(Cart), 1, file);
	}
	fclose(file);
}

//Функції сортування

int compName(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return strcmp(p1->name, p2->name);
}

int compType(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return strcmp(p1->type, p2->type);
}

int compComposition(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return strcmp(p1->composition, p2->composition);
}

int compCountry(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return strcmp(p1->country, p2->country);
}

int compWeight(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	if (p1->weight < p2->weight)
		return -1;
	if (p1->weight > p2->weight)
		return 1;
	return 0;
}

int compPriceBuy(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	if (p1->priceBuy < p2->priceBuy)
		return -1;
	if (p1->priceBuy > p2->priceBuy)
		return 1;
	return 0;
}

int compPriceSell(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	if (p1->priceSell < p2->priceSell)
		return -1;
	if (p1->priceSell > p2->priceSell)
		return 1;
	return 0;
}

int compQuantity(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return  p1->quantity - p2->quantity;
}

int compId(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return  p1->id - p2->id;
}

int compPopularity(const void* a, const void* b) {
	Product* p1 = (Product*)a;
	Product* p2 = (Product*)b;
	return  p1->popularity - p2->popularity;
}

int compEmpty(const void* a, const void* b) {
	const Product* p1 = (const Product*)a;
	const Product* p2 = (const Product*)b;
	if (p1->quantity == 0 && p2->quantity != 0) {
		return 1;
	}
	if (p1->quantity != 0 && p2->quantity == 0) {
		return -1;
	}
	return 0;
}

void sortEmpty(int mode) {
	qsort(massiveProd, num, sizeof(Product), compEmpty);
	if (mode) {
		writeFile(massiveProd);
	}
}

//Функції пошуку

int searchProcess(int found, int mode, int inx) {
	if (!found && mode) {
		puts("id) Íàçâà òîâàðó  òèï êàâè  ñêëàä êàâè  êðà¿íà âèðîáíèêà | ìàñà íåòòî |\n ö³íà çàêóïêè | ö³íà ïðîäàæó | ê³ëüê³ñòü òîâàðó | ïîïóëÿðí³ñòü |");
	}
	found = +1;
	if (!mode) {
		writeHistory(massiveProd[inx]);
		printf("\n%d", inx + 1);
		showUserProduct(massiveProd[inx]);
	}
	else {
		showProduct(massiveProd[inx]);
	}
	return found;
}
void searchName(int mode) {
	char search[N];
	printf("Ïîøóê çà íàçâîþ:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a name '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].name, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nÏîøóê çà íàçâîþ '%s' íå äàâ ðåçóëüòàò³â.\n", search);
}

void searchType(int mode) {
	char search[N];
	printf("Ïîøóê çà òèïîì:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a type '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].type, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nÏîøóê çà òèïîì '%s' íå äàâ ðåçóëüòàò³â.\n", search);
}

void searchComposition(int mode) {
	char search[N];
	printf("Ïîøóê çà ñêëàäîì:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a composition '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].composition, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nÏîøóê çà ñêëàäîì '%s' íå äàâ ðåçóëüòàò³â.\n", search);
}

void searchCountry(int mode) {
	char search[N];
	printf("Ïîøóê çà êðà¿íîþ âèðîáíèêà:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a county '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].country, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nÏîøóê çà êðà¿íîþ âèðîáíèêà '%s' íå äàâ ðåçóëüòàò³â.\n", search);
}

void searchWeight(int mode) {
	float start;
	float finish;
	printf("Ïîøóê çà ìàñîþ íåòòî:");
	do {
		printf("\nÂâåä³òü ìåæ³ ïîøóêó â³ä äî:");
		scanf_s("%f%f", &start, &finish);
	} while (start < 0.0 || finish < 0.0);

	if (start > finish) {
		float copy = finish;
		finish = start;
		start = copy;
	}
	searchMessageF(mode, "\nSearch for the weight from '%.2f' to '%.2f'\n", start, finish);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (massiveProd[inx].weight >= start && massiveProd[inx].weight <= finish)
			found = searchProcess(found, mode, inx);
	}
	noSearchF(found, mode, "\nÏîøóê çà ìàñîþ íåòòî â³ä '%.2f' äî '%.2f' íå äàâ ðåçóëüòàò³â.\n", start, finish);
}

void searchPriceBuy(int mode) {
	float start;
	float finish;
	printf("Ïîøóê çà ö³íîþ çàêóïêè:");
	do {
		printf("\nÂâåä³òü ìåæ³ ïîøóêó â³ä äî:");
		scanf_s("%f%f", &start, &finish);
	} while (start < 0.0 || finish < 0.0);

	if (start > finish) {
		float copy = finish;
		finish = start;
		start = copy;
	}
	int found = 0;
	for (int inx = 0; inx < num; inx++) {
		if (massiveProd[inx].priceBuy >= start && massiveProd[inx].priceBuy <= finish) {
			found = searchProcess(found, mode, inx);
		}
	}
	noSearchF(found, mode, "\nÏîøóê çà ö³íîþ çàêóïêè â³ä '%.2f' äî '%.2f' íå äàâ ðåçóëüòàò³â.\n", start, finish);
}

void searchPriceSell(int mode) {
	float start;
	float finish;
	printf("Ïîøóê çà ö³íîþ ïðîäàæó:");
	do {
		printf("\nÂâåä³òü ìåæ³ ïîøóêó â³ä äî:");
		scanf_s("%f%f", &start, &finish);
	} while (start < 0.0 || finish < 0.0);

	if (start > finish) {
		float copy = finish;
		finish = start;
		start = copy;
	}
	searchMessageF(mode, "\nSearch for the sale price from '%.2f' to '%.2f'\n", start, finish);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (massiveProd[inx].priceSell >= start && massiveProd[inx].priceSell <= finish)
			found = searchProcess(found, mode, inx);
	}
	noSearchF(found, mode, "\nÏîøóê çà ö³íîþ ïðîäàæó â³ä '%.2f' äî '%.2f' íå äàâ ðåçóëüòàò³â.\n", start, finish);
}

void searchQuantity(int mode) {
	int start;
	int finish;
	printf("Ïîøóê çà ê³ëüê³ñòþ òîâàðó:");
	do {
		printf("\nÂâåä³òü ìåæ³ ïîøóêó â³ä äî:");
		scanf_s("%d%d", &start, &finish);
	} while (start < 0 || finish < 0);

	if (start > finish) {
		int copy = finish;
		finish = start;
		start = copy;
	}
	int found = 0;
	for (int inx = 0; inx < num; inx++) {
		if (massiveProd[inx].quantity >= start && massiveProd[inx].quantity <= finish)
			found = searchProcess(found, mode, inx);
	}
	noSearchF(found, mode, "\nÏîøóê çà ê³ëüê³ñòþ â³ä '%d' äî '%d' íå äàâ ðåçóëüòàò³â.\n", start, finish);
}
