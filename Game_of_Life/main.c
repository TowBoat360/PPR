#include <stdio.h>
#include <stdbool.h>
//DUMM DZUMMDDDD
#define ALL_ROWS 5
#define ALL_COLS 6
#define ALL_BYTES ((ALL_ROWS * ALL_COLS + 7) / 8)
// Test um zu schauen obs geht
void print_generation(void);
void set_generation_from_string(const char string[]);
void get_generation_as_string(char string[]);
bool set_next_generation(void);
void game_of_life(int max_generations);
int count_neighbours(int row, int col);
int get_alive(int row, int col);
bool is_set(unsigned char byte, int bit);

const char test_char_1 = 0b11100000; //<< col - 1, danach >> 7 --> 0b00000001, wenn col = 3
const char test_char_2 = 0b10000101; // MIT BIT-UND ARBEITEN!!!!!!!!
const char test_char_3 = 0b11000100;
const char test_char_4 = 0b00001000; //letzten 00 gehören nicht mehr zum Spiel, weil rows * cols = 30 → 4 Byte

unsigned char generation[ALL_BYTES] = {
		test_char_1,
		test_char_2,
		test_char_3,
		test_char_4
};

char test_string[] = "111000001000010111000100000010";

int main()
{
	printf("%d\n", ALL_ROWS);
	printf("%d\n", ALL_COLS);
	printf("%d\n", test_string[0]);
	printf("%d\n", test_string[3]);
	printf("%llu\n", sizeof(test_string));
	printf("%llu\n", sizeof(generation));
	printf("%s\n", test_string);

	set_generation_from_string(test_string);
	print_generation();

	char get_string[ALL_ROWS * ALL_COLS + 1];
	get_generation_as_string(get_string);
	printf("%s\n", get_string);
    printf("\n");

    //bool hallo = set_next_generation();
    //print_generation();

    game_of_life(1);

	return 0;
}

void print_generation(void)
{
	//String, der die Linien zwischen den Zeilen darstellt
	char lines[ALL_COLS * 4 + 2] = { '+' };

	//Laufvariable, die die Stelle im String lines beschreibt
	int condition = 1;

	while (condition <= ALL_COLS * 4)
	{
		lines[condition] = '-';
		lines[condition + 1] = '-';
		lines[condition + 2] = '-';
		lines[condition + 3] = '+';
		condition += 4;
	}

	printf("%s\n", lines);

	int generation_row = 0;
	int generation_col = 0;

	int row_string_col = 1;

	for (int row = 0; row < ALL_ROWS; row++)
	{
		char row_string[ALL_COLS * 4 + 2] = { '|' };

		while (row_string_col <= ALL_COLS * 4)
		{
			if (generation_col == 8)
			{
				generation_col = 0;
				generation_row += 1;
			}

			unsigned char test_char = 0b10000000 >> generation_col;

			row_string[row_string_col] = ' ';
			row_string[row_string_col + 1] = (generation[generation_row] & test_char) > 0 ? 'o' : ' ';
			row_string[row_string_col + 2] = ' ';
			row_string[row_string_col + 3] = '|';


			generation_col += 1;

			row_string_col += 4;
		}
		row_string_col = 1;

		printf("%s\n", row_string);
		printf("%s\n", lines);
	}
}

void set_generation_from_string(const char string[])
{
	int index_string = 0;
	unsigned char temp = 0b00000000;
	for (int i = 0; i < ALL_BYTES; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (string[index_string] == '1')
			{
				temp |= (0b10000000 >> j);
			}

			index_string += 1;
		}

		generation[i] = temp;
		temp = 0b00000000;
	}

	/*unsigned char temp = 0b00000000;
	unsigned char col = 0b10000000;

	int index = 0;
	for (int i = 0; i < (ALL_ROWS * ALL_COLS + 7) / 8; i++)
	{
		for (int j = 0; j < 8; j ++)
		{
			if (string[index + j] == '1')
			{
				temp = temp | col;
			}
			col = col >> 1;
		}

		index += 8;

		generation[i] = temp;

		temp = 0b00000000;
		col = 0b10000000;
	}*/
}

void get_generation_as_string(char string[])
{
	int index = 0;
	for (int i = 0; i < sizeof(generation); i++) //(ALL_ROWS * ALL_COLS + 2) / 8 <- alternativ
	{
		unsigned char row = generation[i];

		for (int j = 0; j < 8 && index < ALL_ROWS * ALL_COLS; j++)
		{
			unsigned char current = row << j;
			current = current >> 7;
			string[index] = current > 0 ? '1' : '0';
			index += 1;
		}
	}
}

bool set_next_generation(void)
{
    int change_counter = 0;

    char string[ALL_ROWS * ALL_COLS + 1];

    int col = 0;
    int row = 0;

	int grid_row = 0;
	int grid_col = 0;

	for (int i = 0; i <= ALL_ROWS * ALL_COLS; i++)
	{
        if(col == 8)
        {
            col = 0;
            row += 1;
        }

		if (grid_col == ALL_COLS - 1)
		{
			grid_row += 1;
			grid_col = 0;
		}

        int count = count_neighbours(grid_row, grid_col);
        bool is_alive = is_set(generation[row], col);

        if(!is_alive && count == 3)
        {
            string[i] = '1';

            change_counter += 1;
        }
        else if (is_alive && (count == 2 || count == 3))
        {
            string[i] = '1';
            change_counter = change_counter;
        }
        else if (is_alive && (count < 2 || count > 3))
        {
            string[i] = '0';
            change_counter += 1;
        }

		grid_col += 1;
        col += 1;
	}

    set_generation_from_string(string);

	return change_counter > 0;
}

int get_alive(int row, int col)
{
    if (row < 0 || col < 0 || row >= ALL_ROWS || col >= ALL_COLS)
    {
        return 0;
    }
    else
    {
        int byte = (row * ALL_COLS + col) / 8;
        int bit = (row * ALL_COLS + col) % 8;

        return (is_set(generation[byte], bit) ? 1 : 0);
    }
}

bool is_set(unsigned char byte, int bit)
{

//    if (((byte << (bit - 1)) >> 7) == 1) //1 = 0b00000001
//    {
//        return true;
//    }

    unsigned char test = 0b10000000;
    return (byte & (test >> bit)) > 0;
}

int count_neighbours(int row, int col)
{
    return get_alive(row - 1, col - 1)
           + get_alive(row - 1, col)
           + get_alive(row - 1, col + 1)
           + get_alive(row, col - 1)
           + get_alive(row, col + 1)
           + get_alive(row + 1, col - 1)
           + get_alive(row + 1, col)
           + get_alive(row + 1, col + 1);

}

void game_of_life(int max_generations)
{
    for (int i = 0; i < max_generations && set_next_generation(); i++)
    {
        print_generation();
    }
}

//neues Leben, wenn genau 3 Nachbaren
//Überleben bei 2 oder 3 Nachbaren
//Sterben, bei weniger als 2 oder mehr als 3 Nachbaren