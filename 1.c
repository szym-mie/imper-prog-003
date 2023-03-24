#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#define N_MAX 100000   // maksymalna liczba prób
#define TEST 1         // 1 - ulatwia testowanie programu, 0 - dla automatycznej oceny

// Oblicza wartość średniej arytmetycznej oraz wariancji próby losowej
void 
aver_varian(const double tab[], size_t n, double *arith_average, double *variance) 
{
	double asum = 0;
	
	for (size_t i = 0; i < n; i++)
		asum += tab[i];

	*arith_average = (asum /= n);

	double vsum = 0;
	double velem;
	for (size_t i = 0; i < n; i++)
	{
		velem = (tab[i] - asum);
		vsum += velem * velem;
	}

	*variance = (vsum /= n);
}

// Wypełnia tablicę tab wynikami n prób Bernoulliego
// 1 - sukces; 0 - porażka
void 
bernoulli_gen(int tab[], size_t n, double probab) 
{
	double limit = probab * (RAND_MAX + 1);
	for (size_t i = 0; i < n; i++)
		tab[i] = rand() < limit;
}

// Dyskretny rozkład prawdopodobieństwa zmiennej losowej - sumy oczek dwóch kostek do gry.
// Wartości zmiennej losowej są liczbami naturalnymi należącymi do przedziału [2-12].
// throws_num - liczba rzutów (jeden rzut to rzut dwoma kostkami).
// Funkcja zapisuje obliczony rozkład w początkowych elementach tablicy tab.
#define OUTCOMES 11
void
two_dice_probab_distrib(double distrib[], int throws_num) 
{
	int counts[OUTCOMES];
	for (size_t i = 0; i < throws_num; i++)
	{
		counts[rand() % 6 + rand() % 6]++;
	}

	for (size_t i = 0; i < OUTCOMES; i++)
		distrib[i] = (double)counts[i] / throws_num;
}

// Oblicza i zwraca Dystrybuantę (Cumulative Distribution Function)
void 
cum_discret_distrib(double distrib[], size_t n) 
{
	double csum = 0;
	for (size_t i = 0; i < n; i++)
		distrib[i] = (csum += distrib[i]);
}

// Histogram - wykres "słupkowy" wartości funkcji zapisanych w tablicy tab o długości n,
// x_start - wartość pierwszej danej (założenie: przyrost zmiennej niezależnej=1),
// y_scale - wartość zmiennej zależnej odpowiadającej szerokości jednego znaku na wykresie,
void 
print_histogram(const double tab[], size_t n, int x_start, double y_scale, char mark) 
{
	// assume no value is grater than 1 (can calculate max length of bar)
	size_t bar_len = (size_t) 1 / y_scale + 2;  // + 2 to ensure float inaccuracy + NUL sign
	char *bar = malloc(bar_len);	
	if (bar == NULL) return;

	for (size_t i = 0; i < bar_len - 1; i++)
		*(bar+i) = mark;
	*(bar+bar_len-1) = '\0';

	for (size_t i = 0; i < n; i++)
	{
		size_t marks = tab[i] / y_scale;
		size_t ptr_diff = bar_len - marks - 1;
		printf("%2zu | %s %.3f\n", i + x_start, bar+ptr_diff, tab[i]);
	}

	free(bar);
}


// Symuluje problem Monty'ego-Hall'a dla przypadku 3-bramkowego
// Wejście: adres licznika wygranych przy zmianie wyboru,
//          adres licznika wygranych bez zamiany,
//          liczba gier (prób).
// Funkcja oblicza ile razy wygrywającym wyborem byłaby zmiana decyzji 
// i ile razy wygrywałaby opcja przeciwna. 
// Założenie: Funkcja wywołuje generator rand dokładnie 2 razy.
//            Pierwsze losowanie: nr drzwi, za którymi jest nagroda.
//            Drugie losowanie: nr drzwi w pierwszym wyborze gracza.
void 
monty_hall(int *p_switch_wins, int *p_nonswitch_wins, int n) 
{
	int prize_door, picked_door;
	
	for (size_t i = 0; i < n; i++)
	{
		prize_door = rand() % 3;
		picked_door = rand() % 3;

		*p_switch_wins += prize_door != picked_door;
		*p_nonswitch_wins += prize_door == picked_door;
	}
}

// read double vector of size n
void read_vector_dbl(double tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%lf", tab + i);
    }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector_dbl(const double tab[], size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%.3f ", tab[i]);
    }
    printf("\n");
}

// print integer vector
void print_vector_int(const int tab[], size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

// get from input stream one character ignoring leading whitespace
char get_char_ignore_w_sp(void) {
    int c;
    do  c = getchar();
    while(isspace(c));
    return c;
}

int main(void) {
    int to_do, seed, number, itab[N_MAX];
    size_t n;
    double dtab[N_MAX], two_dice_tab[11];
    char mark;
    setbuf(stdout, NULL); // prevent buffering - don't want to fflush every printf
			  // windows still doesn't work (don't feel like VS2022)
    if (TEST) printf("Numer testu [1, 7]: ");
    scanf("%d", &to_do);
    if(to_do > 1) {
        if(TEST) printf("seed = ");
        scanf("%d",&seed);
        srand(seed);        // ustawienie ziarna dla powterzalnosci wynikow
    }

    switch(to_do) {
    case 1: // aver_varian
        if (TEST) printf("aver_varian: Wpisz dlugosc tablicy: ");
        scanf("%d", &n);
        if(n < 1 || n > N_MAX) {
            printf("n < 1 lub n > N_MAX\n");
            break;
        }
        { double arith_aver, varian;
            if (TEST) printf("Wpisz elementy tablicy: ");
            read_vector_dbl(dtab, n);
            aver_varian(dtab, n, &arith_aver, &varian);
            if (TEST) printf("średnia arith_aver = ");
            printf("%.3f ", arith_aver);
            if (TEST) printf(" wariancja = ");
            printf("%.3f\n ", varian);
        }
        break;
    case 2: // bernoulli_gen
        if (TEST) printf("Liczba prób: ");
        scanf("%d", &n);
        if(n < 0 || n > N_MAX) {
            printf("n < 0 lub n>%d\n",N_MAX);
            break;
        }
        { double probab;
            if (TEST) printf("Prawdopodobieństwo sukcesu: ");
            scanf("%lf", &probab);
            if(probab < 0 || probab > 1) {
                printf("p<0 lub p>1\n");
                break;
            }
            bernoulli_gen(itab, n, probab);
            print_vector_int(itab, n);
        }
        break;
    case 3:  // probab_distrib i tablica
    case 5:  // probab_distrib i histogram
        if (TEST) printf("Liczba prób: ");
        scanf("%d", &number);
        if(number <= 0 || number > N_MAX) {
            printf("number <= 0 lub number > N_MAX\n");
            break;
        }
        two_dice_probab_distrib(two_dice_tab, number);
        if(to_do==3)  print_vector_dbl(two_dice_tab, 11); 
        else {
            if(TEST) printf("Wpisz znak: ");  
            mark = get_char_ignore_w_sp();  
            print_histogram(two_dice_tab,11,2,0.005,mark);
        }
        break;
    case 4: case 6:  // var_dystryb i hist
        if (TEST) printf("Liczba rzutów dwiema kostkami: ");
        scanf("%d", &number);
        if(number < 0 || number > N_MAX) {
            printf("number <= 0 lub number > N_MAX\n");
            break;
        }
        two_dice_probab_distrib(two_dice_tab, number);
        cum_discret_distrib(two_dice_tab, 11);
        if(to_do==4)  print_vector_dbl(two_dice_tab, 11); 
        else {
            if(TEST) printf("Wpisz znak: ");
            mark = get_char_ignore_w_sp();
            print_histogram(two_dice_tab,11,2,0.02,mark);
        }
        break;
    case 7: // monty_hall
        if (TEST) printf("Wpisz liczbę prób: ");
        scanf("%d", &number);
        if(number <= 0) {
            printf("number<=0\n");
            break;
        }
        { int switch_wins, nonswitch_wins;
            monty_hall(&switch_wins, &nonswitch_wins, number);
            if (TEST) printf("W %d grach zwycięstwo dawała zmiana wyboru drzwi ", number);
            printf("%d ", switch_wins);
            if (TEST) printf("razy,\na po pozostawienie wyboru drzwi bez zmiany  ");
            printf("%d ", nonswitch_wins);
            if (TEST) printf("razy.");
        }
        break;
    default:
        printf("Numer testu spoza zakresu [1, 7] %d", to_do);
        break;
    }
    return 0;
}
