#include "myLib.h"

//Окремі функції зчитування файлів

Product* readFile() {
	FILE* file;
	fopen_s(&file, "Products.txt", "r");
	if (file == NULL) {
		puts("Помилка відкриття файлу \n");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0) {
		printf("Файл пустий, заповніть його!\n");
		fclose(file);
		id += 1;
		num += 1;

		Product* massive = (Product*)malloc(num * sizeof(Product));

		if (massive == NULL) {
			printf("\nДинамічний масив неіснує.\n");
			return NULL;
		}
		massive[0] = initProduct(id);
		writeFile(massive);
		return massive;
	}
	else {
		fseek(file, 0, SEEK_SET);
		int idf;
		int number;
		fread_s(&number, sizeof(int), sizeof(int), 1, file);
		fread_s(&idf, sizeof(int), sizeof(int), 1, file);
		id = idf;
		Product* massive = (Product*)malloc(number * sizeof(Product));
		if (massive == NULL) {
			printf("\nДинамічний масив неіснує.\n");
			exit(1);
		}
		int i = 0;
		while (i < number && fread_s(&massive[i], sizeof(Product), sizeof(Product), 1, file) == 1) {
			i++;
		}

		num = i;
		fclose(file);
		return massive;
	}
}

int readCassaNum() {
	FILE* file;
	fopen_s(&file, "CassaNum.txt", "r");
	if (file == NULL) {
		puts("Помилка відкриття файлу \n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0) {
		fclose(file);
		return 0;
	}
	else {
		fseek(file, 0, SEEK_SET);
		int number = 0;
		fread_s(&number, sizeof(int), sizeof(int), 1, file);
		fclose(file);
		return number;
	}
}

//Функції для різних перевірок

int checkChoice(int choice) {
	if (choice == 0) {
		return choice;
	}
	for (int i = 0; i < num; i++) {
		if (massiveProd[i].id == choice) {
			choice = i + 1;
			return choice;
		}
	}
	return -1;
}

int checkPass() {
	char password[N];
	char test[N];
	FILE* file;
	fopen_s(&file, "Admin.txt", "r");

	if (file == NULL) {
		puts("Помилка відкриття файлу \n");
		exit(1);
	}

	fread_s(&password, sizeof(password), sizeof(password), 1, file);
	fclose(file);
	printf("Введіть пароль (10 сим макс):");
	scanf_s("%s", test, N);
	if (strcmp(password, test) != 0) {
		printf("\nНевірний пароль!!!\n");
		writeSecurity(0, test);
		return 0;
	}
	else {
		writeSecurity(1, test);
		return 1;
	}
}

int checkAdmin() {
	FILE* file;
	fopen_s(&file, "Admin.txt", "r");
	if (file == NULL) {
		puts("Помилка відкриття файлу \n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0) {
		fclose(file);
		return 0;
	}
	else {
		fclose(file);
		return 1;
	}
}
int checkEmpty() {
	int empty = 0;
	for (int i = 0; i < num; i++) {
		if (massiveProd[i].quantity == 0) {
			empty += 1;
		}
	}
	sortEmpty(1);
	return empty;
}

//Функції адміністратора

Product* addProduct() {
	id += 1;
	num += 1;

	Product* massiveAdd = (Product*)malloc(num * sizeof(Product));
	if (massiveAdd == NULL) {
		printf("\nДинамічний масив неіснує.\n");
		exit(1);
	}
	for (int i = 0; i < num - 1; i++) {
		massiveAdd[i] = massiveProd[i];
	}
	massiveAdd[num - 1] = initProduct(id);
	showArr(massiveAdd);
	writeFile(massiveProd);
	free(massiveProd);
	return massiveAdd;
}

Product* delProduct(int choice) {
	if (choice == 0) {
		printf("Неправильний вибір!!!\n");
		return massiveProd;
	}
	if (num <= 1) {
		printf("Видалення не можливе!!!\n");
		showArr(massiveProd);
		return massiveProd;
	}
	writeDel(massiveProd[choice - 1]);
	for (int i = choice - 1; i < num - 1; i++) {
		massiveProd[i] = massiveProd[i + 1];
	}

	num -= 1;
	Product* massiveDel = (Product*)malloc(num * sizeof(Product));
	if (massiveDel == NULL) {
		printf("\nДинамічний масив неіснує.\n");
		exit(1);
	}
	for (int i = 0; i < num; i++) {
		massiveDel[i] = massiveProd[i];
	}
	printf("\nВидалено дані %dго товару.\n", choice);
	showArr(massiveDel);
	writeFile(massiveProd);
	free(massiveProd);
	return massiveDel;
}

void editProduct(int choice) {
	if (choice == 0) {
		printf("Неправильний вибір!!!\n");
		return;
	}
	int comd;
	printf("Редагування %d товару:", massiveProd[choice - 1].id);
	showEdit(massiveProd[choice - 1]);
	do {
		puts("\nВиберіть рядок який хочете змінити,Повернутися - 0,Змінити повністю - 9");
		scanf_s("%d", &comd);
		switch (comd)
		{
		case 0:
			break;
		case 1:
			printf("Введіть назву:");
			scanf_s("%s", massiveProd[choice - 1].name, STRING);
			showEdit(massiveProd[choice - 1]);
			break;
		case 2:
			printf("Введіть тип кави (мелена,зерно...):");
			scanf_s("%s", massiveProd[choice - 1].type, STRING);
			showEdit(massiveProd[choice - 1]);
			break;
		case 3:
			printf("Введіть склад кави (арабіка,робуста...):");
			scanf_s("%s", massiveProd[choice - 1].composition, STRING);
			showEdit(massiveProd[choice - 1]);
			break;

		case 4:
			printf("Введіть країну виробника:");
			scanf_s("%s", massiveProd[choice - 1].country, STRING);
			showEdit(massiveProd[choice - 1]);
			break;
		case 5:
			do {
				printf("Введіть масу нетто в грамах:");
				scanf_s("%f", &massiveProd[choice - 1].weight);
			} while (massiveProd[choice - 1].weight <= 0);
			showEdit(massiveProd[choice - 1]);
			break;
		case 6:
			do {
				printf("Введіть ціну закупки:");
				scanf_s("%f", &massiveProd[choice - 1].priceBuy);
			} while (massiveProd[choice - 1].priceBuy <= 0.0);
			showEdit(massiveProd[choice - 1]);
			break;
		case 7:
			do {
				printf("Введіть ціну продажу:");
				scanf_s("%f", &massiveProd[choice - 1].priceSell);
			} while (massiveProd[choice - 1].priceSell <= massiveProd[choice - 1].priceBuy);
			showEdit(massiveProd[choice - 1]);
			break;
		case 8:
			do {
				printf("Введіть кількість товару:");
				scanf_s("%d", &massiveProd[choice - 1].quantity);
			} while (massiveProd[choice - 1].quantity < 0);
			showEdit(massiveProd[choice - 1]);
			break;
		case 9:
			printf("Повна заміна:");
			massiveProd[choice - 1] = initProduct(choice);
			showEdit(massiveProd[choice - 1]);
			break;
		default:
			printf("Неправильний вибір!!!");
			break;
		}
	} while (comd != 0);
	writeFile(massiveProd);
	showArr(massiveProd);
}

void sortProduct(int mode) {
	if (mode) {
		printf("\n1 - назва, 2 - тип кави, 3- склад кави, 4 - країна виробника, 5 - маса нетто");
		printf("\n6 - ціна продажу, 7 - за популярністю , 8 - кількість, 9 - ціна закупки ,10 - id\n");
	}
	else {
		printf("\n1 - назва, 2 - тип кави, 3- склад кави, 4 - країна виробника, 5 - маса нетто");
		printf("\n6 - ціна товару, 7 - за популярністю\n");
	}
	int choice = 0;
	int flagChoice;
	do {
		printf("Виберіть за яким полем буде сортування:");
		flagChoice = scanf_s("%d", &choice);
		if (flagChoice != 1) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	} while (flagChoice != 1);

	if (!mode && choice > 7) {
		printf("Неправильний вибір!!!");
		return;
	}
	switch (choice) {
	case 1:
		printf("\nСортування за назвою виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compName);
		break;
	case 2:
		printf("\nСортування за типом виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compType);
		break;
	case 3:
		printf("\nСортування за складом виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compComposition);
		break;
	case 4:
		printf("\nСортування за країною виробника виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compCountry);
		break;
	case 5:
		printf("\nСортування за вагою виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compWeight);
		break;
	case 6:
		printf("\nСортування за ціною виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compPriceSell);
		break;
	case 7:
		printf("\nСортування за популярністю виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compPopularity);
		break;
	case 8:
		printf("\nСортування за кількістю товару виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compQuantity);
		break;
	case 9:
		printf("\nСортування за ціною закупки виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compPriceBuy);
		break;
	case 10:
		printf("\nСортування за Id виконане:\n");
		qsort(massiveProd, num, sizeof(Product), compId);
		break;
	default:
		printf("Неправильний вибір!!!");
		return;
	}

	if (mode) {
		writeFile(massiveProd);
		showArr(massiveProd);
	}
	else {
		sortEmpty(0);
		showUserArr();
	}
}

void searchProduct(int mode) {
	if (mode) {
		printf("\n1 - назва, 2 - тип кави, 3- склад кави, 4 - країна виробника, 5 - маса нетто");
		printf("\n6 - ціна продажу, 7 - ціна закупки, 8 - кількість\n");
	}
	else {
		printf("\n1 - назва, 2 - тип кави, 3- склад кави, 4 - країна виробника, 5 - маса нетто,6 - ціна товару\n");
	}

	int choice = 0;
	int flagChoice;
	do {
		printf("Виберіть за яким полем буде пошук:");
		flagChoice = scanf_s("%d", &choice);
		if (flagChoice != 1) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	} while (flagChoice != 1);

	if (!mode && choice > 6) {
		printf("Неправильний вибір!!!");
		return;
	}

	switch (choice) {
	case 1:
		searchName(mode);
		break;
	case 2:
		searchType(mode);
		break;
	case 3:
		searchComposition(mode);
		break;
	case 4:
		searchCountry(mode);
		break;
	case 5:
		searchWeight(mode);
		break;
	case 6:
		searchPriceSell(mode);
		break;
	case 7:
		searchPriceBuy(mode);
		break;
	case 8:
		searchQuantity(mode);
		break;
	default:
		printf("Неправильний вибір!!!");
		return;
	}
}

void cashCounting() {
	if (cassa == 0) {
		printf("\nЗаписів покупок поки що немає!\n");
		return;
	}
	FILE* file;
	fopen_s(&file, "Cassa.txt", "r");

	if (file == NULL) {
		puts("Помилка відкриття файлу \n");
		exit(1);
	}

	Cart* massive = (Cart*)malloc(cassa * sizeof(Cart));
	if (massive == NULL) {
		printf("\nДинамічний масив неіснує.\n");
		exit(1);
	}
	int i = 0;
	while (i < cassa && fread_s(&massive[i], sizeof(Cart), sizeof(Cart), 1, file) == 1) {
		i++;
	}
	fclose(file);
	float allCassa = 0;
	float allDisperse = 0;
	for (int j = 0; j < cassa; j++) {
		allCassa += massive[j].priceAll;
		allDisperse += (massive[j].quantityCart * massive[j].product.priceBuy);
	}
	float profit = allCassa - allDisperse;
	showCartArr(massive, cassa, 0);
	printf("\n\nЗагальна касса - %.2f грн.", allCassa);
	printf("\nВитрати на проданий товар - %.2f грн.", allDisperse);
	printf("\nЧистий прибуток - %.2f грн.\n", profit);
	free(massive);
}

void clearData() {
	FILE* file1 = fopen("Products.txt", "w");
	if (file1 == NULL) {
		perror("Помилка відкриття Products.txt");
		exit(1);
	}
	fclose(file1);

	FILE* file2 = fopen("Security.txt", "w");
	if (file2 == NULL) {
		perror("Помилка відкриття Security.txt");
		exit(1);
	}
	fclose(file2);

	FILE* file3 = fopen("searchHistory.txt", "w");
	if (file3 == NULL) {
		perror("Помилка відкриття searchHistory.txt");
		exit(1);
	}
	fclose(file3);

	FILE* file4 = fopen("Deleted.txt", "w");
	if (file4 == NULL) {
		perror("Помилка відкриття Deleted.txt");
		exit(1);
	}
	fclose(file4);

	FILE* file5 = fopen("CassaNum.txt", "w");
	if (file5 == NULL) {
		perror("Помилка відкриття CassaNum.txt");
		exit(1);
	}
	fclose(file5);

	FILE* file6 = fopen("Cassa.txt", "w");
	if (file6 == NULL) {
		perror("Помилка відкриття Cassa.txt");
		exit(1);
	}
	fclose(file6);

	FILE* file7 = fopen("Admin.txt", "w");
	if (file7 == NULL) {
		perror("Помилка відкриття Admin.txt");
		exit(1);
	}
	fclose(file7);
	printf("Дані файлів видалено!\n");
	exit(0);
}

Product* adminMenu(int mode) {
	massiveProd = readFile();
	showArr(massiveProd);

	char command[N];
	int choice;
	int flagChoice;
	int flagEx, flagEd, flagDel, flagAdd, flagSort, flagCassa, flagSearch, flagClear, errInp;
	do {
		puts("\nВийти - exit 0,Редагувати - edit №Product,Видалити - del №Product,Додати - add 0,\nСортування - sort 0,Пошук - search 0,Касса - cassa 0,Очистити данні - clear 0");
		do {
			flagEx = 0; flagEd = 0; flagDel = 0; flagAdd = 0, flagSort = 0, flagCassa = 0, flagClear = 0; flagSearch = 0, errInp = 0;
			scanf_s("%s", &command, N);
			flagChoice = scanf_s("%d", &choice);
			choice = checkChoice(choice);

			if (strcmp(command, "exit") == 0)
				flagEx++;

			else if (strcmp(command, "edit") == 0)
				flagEd++;

			else if (strcmp(command, "del") == 0)
				flagDel++;

			else if (strcmp(command, "add") == 0)
				flagAdd++;

			else if (strcmp(command, "sort") == 0)
				flagSort++;

			else if (strcmp(command, "search") == 0)
				flagSearch++;

			else if (strcmp(command, "cassa") == 0)
				flagCassa++;

			else if (strcmp(command, "clear") == 0)
				flagClear++;

			else {
				errInp++;
				printf("Неправильна команда!\n");
			}
		} while (errInp == 1 || choice < 0 || choice > num || flagChoice != 1);

		if (flagEd)
			editProduct(choice);

		else if (flagSort)
			sortProduct(1);

		else if (flagSearch)
			searchProduct(1);

		else if (flagDel)
			massiveProd = delProduct(choice);

		else if (flagAdd) {
			massiveProd = addProduct();
		}
		else if (flagCassa) {
			cashCounting();
			showArr(massiveProd);
		}

		else if (flagClear) {
			if (checkPass()) {
				clearData();
			}
			else {
				continue;
			}
		}
	} while (flagEx != 1);
	empty = checkEmpty();
	if (mode)
		showUserArr();
	return massiveProd;
}

//Функції користувача

Cart* addCart(Product Product, int quantity) {
	for (int i = 0; i < cart; i++) {
		if (Product.id == massiveCart[i].product.id) {
			printf("\nТовар Кава %s %s %.2f гр. вже є у кошику!\n", Product.name, Product.type, Product.weight);
			return massiveCart;
		}
	}

	cart += 1;
	Cart* massiveAdd = (Cart*)malloc(cart * sizeof(Cart));
	if (massiveAdd == NULL) {
		printf("\nДинамічний масив неіснує.\n");
		exit(1);
	}
	for (int i = 0; i < cart - 1; i++) {
		massiveAdd[i] = massiveCart[i];
	}
	massiveAdd[cart - 1] = initCart(Product, quantity);
	totalPrice += massiveAdd[cart - 1].priceAll;
	printf("\nТовар Кава %s %s %.2f гр. кількістю %d додано у кошик\n", Product.name, Product.type, Product.weight, quantity);
	free(massiveCart);
	return massiveAdd;
}

Cart* delCart(int choice) {
	for (int i = choice - 1; i < cart - 1; i++) {
		massiveCart[i] = massiveCart[i + 1];
	}
	cart -= 1;
	Cart* massiveDel = (Cart*)malloc(cart * sizeof(Cart));
	if (massiveDel == NULL) {
		printf("\nДинамічний масив неіснує.\n");
		exit(1);
	}
	for (int i = 0; i < cart; i++) {
		massiveDel[i] = massiveCart[i];
	}
	printf("\nВидалено товар №%d з кошику.\n", choice);
	float totalPrice1 = 0;
	for (int i = 0; i < cart; i++) {
		totalPrice1 += massiveDel[i].priceAll;
	}
	totalPrice = totalPrice1;
	free(massiveCart);
	showCartArr(massiveDel, cart, 1);
	return massiveDel;
}

void editCart(int choice) {
	int newQuantity;
	printf("\nВведіть нову кількість:");
	scanf_s("%d", &newQuantity);
	if (newQuantity < 0 || newQuantity > massiveCart[choice - 1].product.quantity) {
		printf("\nТакої кількості товару під номером %d немає!!!", choice);
		return;
	}

	massiveCart[choice - 1].quantityCart = newQuantity;
	massiveCart[choice - 1].priceAll = newQuantity * massiveCart[choice - 1].product.priceSell;
	float totalPrice1 = 0;
	for (int i = 0; i < cart; i++) {
		totalPrice1 += massiveCart[i].priceAll;
	}
	totalPrice = totalPrice1;
	showCartArr(massiveCart, cart, 1);
}

void payCart() {

	for (int i = 0; i < cart; i++) {
		for (int j = 0; j < num; j++) {
			if (massiveCart[i].product.id == massiveProd[j].id) {
				massiveProd[j].quantity -= massiveCart[i].quantityCart;
				massiveProd[j].popularity += 1;
				break;
			}
		}
	}
	printf("\nВаш товар оплачено.Дякуюємо за покупку!\n");
	qsort(massiveProd, num, sizeof(Product), compId);
	writeFile(massiveProd);
	cassa = readCassaNum();
	int num = cassa + cart;
	FILE* file;
	fopen_s(&file, "CassaNum.txt", "w");
	if (file == NULL) {
		printf("Failed to open file \n");
		exit(1);
	}
	fwrite(&num, sizeof(num), 1, file);
	fclose(file);
	cassa = num;
	empty = checkEmpty();
	writeCassa();
}

Cart* cartMenu() {
	if (cart == 0) {
		printf("\nКошик порожній,поповніть його.\n");
		return massiveCart;
	}
	else {
		printf("\nВаш кошик:\n");
		showCartArr(massiveCart, cart, 1);
	}
	char command[N];
	int choice;
	int flagChoice;
	int flagEx, flagEd, flagDel, flagPay, errInp;
	do {
		puts("\nПовернутися - exit 1,Редагувати кількість товару- edit №Cart,Видалити - del №Cart,Оплатити - pay 1");
		do {
			flagEx = 0; flagEd = 0; flagDel = 0; flagPay = 0; errInp = 0;
			scanf_s("%s", &command, N);
			flagChoice = scanf_s("%d", &choice);

			if (strcmp(command, "exit") == 0)
				flagEx++;

			else if (strcmp(command, "edit") == 0)
				flagEd++;

			else if (strcmp(command, "del") == 0)
				flagDel++;

			else if (strcmp(command, "pay") == 0)
				flagPay++;

			else {
				errInp++;
				printf("Неправильна команда!\n");
			}
		} while (errInp == 1 || choice < 1 || choice > cart || flagChoice != 1);

		if (flagEd)
			editCart(choice);

		else if (flagDel) {
			massiveCart = delCart(choice);
			if (cart == 0)
				break;
		}

		else if (flagPay) {
			payCart();
			cart = 0;
			massiveCart = NULL;
			totalPrice = 0;
			break;
		}

	} while (flagEx != 1);
	showUserArr();
	return massiveCart;
}

int main() {
	SetConsoleOutputCP(1251);
	cassa = readCassaNum();
	if (!checkAdmin()) {
		char pass1[N];
		char pass2[N];
		int check = 0;
		printf("\nВітаємо новий адмін!\n");
		int flag = 1;
		while (flag) {
			printf("Введіть пароль (10 сим):");
			scanf_s("%s", pass1, N);
			printf("Підтвердіть пароль:");
			scanf_s("%s", pass2, N);
			if (strcmp(pass1, pass2) == 0) {
				flag = 0;
				break;
			}
		}
		printf("Пароль успішно підтверджено!\n");
		FILE* file;
		fopen_s(&file, "Admin.txt", "w");
		if (file == NULL) {
			puts("Помилка відкриття файлу\n");
			exit(1);
		}
		fwrite(&pass1, sizeof(pass1), 1, file);
		fclose(file);
		massiveProd = readFile();
		adminMenu(0);
	}
	printf("Вітаємо в CoffeeMachine!\nНаш товар:\n");
	massiveProd = readFile();
	empty = checkEmpty();
	showUserArr();
	char command[N];
	int flagEx, flagAdd, flagCart, flagSort, flagSearch, flagAdmin, errInp;
	do {
		puts("\nВийти - exit,Сортування - sort,Пошук - search\nДодати в корзину - add,Відкрити кошик - cart");
		do {
			flagEx = 0, flagAdd = 0, flagCart = 0, flagSort = 0, flagSearch = 0, flagAdmin = 0, errInp = 0;
			scanf_s("%s", &command, N);

			if (strcmp(command, "exit") == 0)
				flagEx++;

			else if (strcmp(command, "admin") == 0)
				flagAdmin++;

			else if (strcmp(command, "add") == 0)
				flagAdd++;

			else if (strcmp(command, "cart") == 0)
				flagCart++;

			else if (strcmp(command, "sort") == 0)
				flagSort++;

			else if (strcmp(command, "search") == 0)
				flagSearch++;

			else {
				errInp++;
				printf("Неправильна команда!\n");
			}
		} while (errInp == 1);

		if (flagSort)
			sortProduct(0);

		else if (flagSearch)
			searchProduct(0);

		else if (flagAdd) {
			int choice = 0, quantity = 0;
			printf("Введіть номер товару та кількість товару: ");
			scanf_s("%d%d", &choice, &quantity);
			if (choice > num - empty || choice <= 0) {
				printf("\nТовару під таким номером не існує!!!\n");
				continue;
			}
			Product Product = massiveProd[choice - 1];
			if (quantity <= 0 || quantity > Product.quantity) {
				printf("\nТакої кількості товару під номером %d немає!!!\n", choice);
				continue;
			}
			massiveCart = addCart(Product, quantity);
		}

		else if (flagCart) {
			massiveCart = cartMenu();
		}

		else if (flagAdmin) {
			if (checkPass()) {
				massiveProd = adminMenu(1);
			}
			else
				showUserArr();
		}
	} while (flagEx != 1);
	free(massiveCart);
	free(massiveProd);
	return 0;
}
