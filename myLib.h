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
	puts("Введіть інформацію про товар\n");
	Product.id = id;
	Product.popularity = 0;

	printf("Введіть назву:");
	scanf_s("%s", Product.name, STRING);

	do {
		printf("Введіть ціну закупки:");
		scanf_s("%f", &Product.priceBuy);
	} while (Product.priceBuy <= 0.0);

	do {
		printf("Введіть ціну продажу:");
		scanf_s("%f", &Product.priceSell);
	} while (Product.priceSell <= Product.priceBuy);

	do {
		printf("Введіть кількість товару:");
		scanf_s("%d", &Product.quantity);
	} while (Product.quantity <= 0);

	do {
		printf("Введіть масу нетто в грамах:");
		scanf_s("%f", &Product.weight);
	} while (Product.weight <= 0);

	printf("Введіть тип кави (мелена,зерно...):");
	scanf_s("%s", Product.type, STRING);

	printf("Введіть країну виробника:");
	scanf_s("%s", Product.country, STRING);

	printf("Введіть склад кави (арабіка,робуста...):");
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
	printf("\n%d) %s  %s  %s  %s | вага %.2f гр.|\nзак. %.2f грн. | прод. %.2f грн. | кільк. %d | поп. %d |\n", Product.id, Product.name,
		Product.type, Product.composition, Product.country, Product.weight, Product.priceBuy, Product.priceSell, Product.quantity, Product.popularity);
}

void showArr(Product* massive)
{
	if (num <= 0) {
		printf("Список пустий!\n");
		return;
	}
	puts("\nid) Назва товару  тип кави  склад кави  країна виробника | маса нетто |\n ціна закупки | ціна продажу | кількість товару | популярність |");

	for (int i = 0; i < num; i++)
	{
		showProduct(massive[i]);
	}
}

void showUserProduct(Product Product)
{
	printf(")Кава %s %s %.2f гр. - %.2f грн.\n%s %s на складі: %d шт.\n", Product.name,
		Product.type, Product.weight, Product.priceSell, Product.composition, Product.country, Product.quantity);
}

void showUserArr()
{
	if (num <= 0 || num - empty == 0) {
		printf("Список пустий!\n");
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
	printf(")Кава %s %s %.2f гр. - %dшт. x %.2f грн.- %.2f грн.\n%s %s\n", Cart.product.name,
		Cart.product.type, Cart.product.weight, Cart.quantityCart, Cart.product.priceSell, Cart.priceAll, Cart.product.composition, Cart.product.country);
}

void showCartArr(Cart* massive, int num, int mode)
{
	if (num <= 0 && mode) {
		printf("Кошик порожній,поповніть його.\n");
		return;
	}

	for (int i = 0; i < num; i++)
	{
		printf("\n%d", i + 1);
		showCart(massive[i]);
	}
	if (mode) {
		printf("\n________________________ Загальна ціна - %.2f грн.\n", totalPrice);
	}
}

void showEdit(Product Product)
{
	printf("\n1)Назва: %s\n2)Тип: %s\n3)Склад: %s\n4)Країна: %s\n5)Вага: %.2f\n6)Ціна закупки: %.2f\n7)Ціна продажу: %.2f\n8)Кількість: %d\n", Product.name,
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
		fprintf(file, "id) Назва товару  тип кави  склад кави  країна виробника | маса нетто |\n ціна закупки | ціна продажу | кількість товару | популярність |\n");
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
		puts("id) Назва товару  тип кави  склад кави  країна виробника | маса нетто |\n ціна закупки | ціна продажу | кількість товару | популярність |");
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
	printf("Пошук за назвою:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a name '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].name, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nПошук за назвою '%s' не дав результатів.\n", search);
}

void searchType(int mode) {
	char search[N];
	printf("Пошук за типом:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a type '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].type, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nПошук за типом '%s' не дав результатів.\n", search);
}

void searchComposition(int mode) {
	char search[N];
	printf("Пошук за складом:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a composition '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].composition, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nПошук за складом '%s' не дав результатів.\n", search);
}

void searchCountry(int mode) {
	char search[N];
	printf("Пошук за країною виробника:");
	scanf_s("%s", search, N);
	searchMessage(mode, "\nSearch for a county '%s'\n", search);

	int found = 0;
	for (int inx = 0; inx < num - empty; inx++) {
		if (strncmp(massiveProd[inx].country, search, strlen(search)) == 0)
			found = searchProcess(found, mode, inx);
	}
	noSearch(found, mode, "\nПошук за країною виробника '%s' не дав результатів.\n", search);
}

void searchWeight(int mode) {
	float start;
	float finish;
	printf("Пошук за масою нетто:");
	do {
		printf("\nВведіть межі пошуку від до:");
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
	noSearchF(found, mode, "\nПошук за масою нетто від '%.2f' до '%.2f' не дав результатів.\n", start, finish);
}

void searchPriceBuy(int mode) {
	float start;
	float finish;
	printf("Пошук за ціною закупки:");
	do {
		printf("\nВведіть межі пошуку від до:");
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
	noSearchF(found, mode, "\nПошук за ціною закупки від '%.2f' до '%.2f' не дав результатів.\n", start, finish);
}

void searchPriceSell(int mode) {
	float start;
	float finish;
	printf("Пошук за ціною продажу:");
	do {
		printf("\nВведіть межі пошуку від до:");
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
	noSearchF(found, mode, "\nПошук за ціною продажу від '%.2f' до '%.2f' не дав результатів.\n", start, finish);
}

void searchQuantity(int mode) {
	int start;
	int finish;
	printf("Пошук за кількістю товару:");
	do {
		printf("\nВведіть межі пошуку від до:");
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
	noSearchF(found, mode, "\nПошук за кількістю від '%d' до '%d' не дав результатів.\n", start, finish);
}
