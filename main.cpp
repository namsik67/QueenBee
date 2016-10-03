#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define RULE_STRING 27
#define RULE_STRING_HOLDER RULE_STRING + 2
#define RULE_PATTERN_HOLDER 27

struct hive {
	int size;
	int *matrix;
};

int get_at(struct hive *p_hive, int x, int y)
{
	return p_hive->matrix[y * p_hive->size + x];
}

void print_hive(struct hive *p_hive)
{
	int i, j, size = p_hive->size;
	int *matrix = p_hive->matrix;
	for (j = 0; j < size; ++j) {
		for (i = 0; i < size; ++i)
			printf("%d ", *matrix++);
		printf("\n");
	}
}

struct rule {
	char string[RULE_STRING_HOLDER];
};

struct day_growth {
	int zero_count;
	int one_count;
	int two_count;
};

struct growth_count {
	int zero_count;
	int one_count;
	int two_count;
	int undefined;
};

struct config {
	/* from input */
	int size;
	int nDays;
	struct rule *rule;
	struct day_growth *growth;

	/* calculated */
	int rule_number;
	int matrix_size;
};

int load_config(struct config *p_config)
{
	int i, rule_number;
	struct rule *p_rule;
	struct day_growth *p_growth;

	/* hive size and days to grow */
	scanf("%d %d\n", &p_config->size, &p_config->nDays);

	/* growth rule for position and neighbor environment */
	rule_number = (p_config->size - 1);
	rule_number *= rule_number;
	p_config->rule_number = rule_number;
	p_rule = (struct rule *)malloc(sizeof(struct rule) * rule_number);
	p_config->rule = p_rule;
	for (i = 0; i < rule_number; ++i, ++p_rule) {
		fgets(p_rule->string, RULE_STRING_HOLDER, stdin);
	}
	
	/* initial growth vector for first row and first column */
	p_growth = (struct day_growth *)malloc(sizeof(struct day_growth) * p_config->nDays);
	memset(p_growth, 0, sizeof(struct day_growth) * p_config->nDays);
	p_config->growth = p_growth;
	for (i = 0; i < p_config->nDays; ++i, ++p_growth) {
		scanf("%d %d %d\n", &p_growth->zero_count, &p_growth->one_count, &p_growth->two_count);
	}
	p_config->matrix_size = p_config->size;
	p_config->matrix_size *= p_config->matrix_size;
	return 0;
}

int verify_config(struct config *p_config)
{
	int i, rule_number;
	printf("%d %d\n", p_config->size, p_config->nDays);
	for (i = 0; i < p_config->rule_number; ++i) {
		printf("len(rule[%d]) = %d\n", i, strlen(p_config->rule[i].string));
	}
	for (i = 0; i < p_config->nDays; ++i) {
		printf("sum(day[%d]'s growth) = %d\n", i, p_config->growth[i].zero_count + p_config->growth[i].one_count + p_config->growth[i].two_count);
	}
	return 0;
}

/* valid case */
/* L <= D <= U : initially fill 0, 1, 2 in order of left side, upper-left corner, upper side */
/* L  D  U : offset in rule_string */
/* 0  0  0 : 0    ==> CASE A */
/* 0  0  1 : 1    ==> CASE C */
/* 0  0  2 : 2    ==> CASE C */
/* 0  1  1 : 4    ==> CASE C */
/* 0  1  2 : 5    ==> CASE C */
/* 0  2  2 : 8    ==> CASE C */
/* 1  1  1 : 13   ==> CASE A */
/* 1  1  2 : 14   ==> CASE B */
/* 1  2  2 : 17   ==> CASE B */
/* 2  2  2 : 26   ==> CASE A */
int value(char *rule_string, int left, int diagonal, int up)
{
	/* Case A */
	if (left == diagonal && diagonal == up)
		return left; /* whatever lava mimics, it is the same. */
	
	if (left == 0) {
		/* Case C */
		if (diagonal == 2) {
			if (rule_string[8] == 'L')
				return 0;
			return 2;
		}
		else if (diagonal == 0) {
			if (rule_string[up] == 'U')
				return up;
			return 0;
		}
		else {
			if (up == 1) {
				if (rule_string[4] == 'L')
					return 0;
				return 1;
			}
			char c;
			if ((c = rule_string[5]) == 'L')
				return 0;
			else if (c == 'D')
				return 1;
			else
				return 2;
		}
			
	}
	else { /* left == 1 since left never become 2 except the case when all three are 2. */
		/* CASE B */
		if (diagonal == 2) {
			if (rule_string[17] == 'L')
				return 1;
			else
				return 2;
		}
		else {/* diagonal == 1 && up == 2 */
			if (rule_string[14] == 'U')
				return 2;
			else
				return 1;
		}
	}
	return -1;
}

int revise_rule(struct rule *rule, int nRule)
{
	/* do nothing : lazy evaluation */
	return 0;
}

int *init_matrix(int matrix_size)
{
	int i;
	int *matrix = (int *)malloc(sizeof(int) * matrix_size), *p;
	for (i = 0, p = matrix; i < matrix_size; ++i, ++p)
		*p = 1;
	return matrix;
}

void set_row(struct growth_count *row, int zero_count, int one_count, int two_count, int undefined)
{
	row->zero_count = zero_count;
	row->one_count = one_count;
	row->two_count = two_count;
	row->undefined = undefined;
}

int fill_first_col_row(struct growth_count *rows, struct day_growth *p_day_growth, int size)
{
	int t1, t2, i;
	if (p_day_growth->zero_count >= size) {
		t1 = p_day_growth->zero_count - size + 1;
		set_row(rows, t1, p_day_growth->one_count, p_day_growth->two_count, 0);
		for (i = 1; i < size; ++i) {
			set_row(rows + i, t1, 0, 0, size - t1);
		}
	}
	else if (p_day_growth->two_count > size) {
		t1 = p_day_growth->two_count - size + 1;
		for (i = 0; i < t1; ++i)
			set_row(rows + i, 0, 0, size, 0);
		for (i = 0; i < p_day_growth->one_count; ++i, ++t1)
			set_row(rows + t1, 0, 1, 0, size - 1);
		for (i = 0; i < p_day_growth->zero_count; ++i, ++t1)
			set_row(rows + t1, 1, 0, 0, size - 1);
	}
	else {
		t1 = size - p_day_growth->two_count;
		t2 = p_day_growth->one_count - t1;
		set_row(rows, 0, t1, p_day_growth->two_count, 0);
		t1 = 1;
		for (i = 0; i < t2; ++i, ++t1)
			set_row(rows + t1, 0, 1, 0, size - 1);
		for (i = 0; i < p_day_growth->zero_count; ++i, ++t1)
			set_row(rows + t1, 1, 0, 0, size - 1);
	}
	return 0;
}

//#define DEBUG_REST
int fill_rest(struct growth_count *rows, struct rule *rule, int size)
{
	int i, j, k, first_col, sum_01, zero_cnt, one_cnt;
	int left = -1, diagonal = -1, up = -1, my_value;
	struct growth_count *prev_row = rows, *curr_row = rows + 1;
	struct rule *p_rule = NULL;
	for (i = 1; i < size; ++i, prev_row = curr_row++) {
#ifdef DEBUG_REST
		printf("row = %d\n", i);
#endif
		zero_cnt = 0;
		one_cnt = 0;
		if (curr_row->undefined == 0)
			continue;
		if (curr_row->two_count != 0)
			continue;
		// 0인 컬럼은 보지 않아도 되기 때문에 정의되지 않은 첫번째 컬럼은 0인 컬럼의 수와 동일하다.
		j = first_col = curr_row->zero_count + curr_row->one_count;
		left = curr_row->one_count;
		sum_01 = prev_row->zero_count + prev_row->one_count;
		p_rule = rule + (i - 1) * (size - 1) + j - 1;
		if (prev_row->zero_count == first_col) {
			diagonal = 0;
			if (prev_row->one_count == 0) 
				up = 2;
			else 
				up = 1;
			my_value = value(p_rule->string, left, 0, up);
#ifdef DEBUG_REST
			printf("rule[(%d,%d) = %d] = %s left = %d diagonal = %d up = %d my_value = %d\n",
				i, j, (i - 1) * (size - 1) + j - 1, rule[(i - 1) * (size - 1) + j - 1].string, left, 0, up,
				my_value);
#endif
			if (my_value == 0)
				++zero_cnt;
			else if (my_value == 1) {
				++one_cnt;
			}
			else {
				if (curr_row->undefined) {
					curr_row->two_count = curr_row->undefined - zero_cnt - one_cnt;
					goto row_done;
				}
			}
			left = my_value;
			++j;
			++p_rule;
		}
		for (; j < sum_01; ++j, ++p_rule) {
			//printf("current_col = %d left = %d diagonal = %d up = %d\n", j, left, 1, 1);
			if (left == 1) {
				++one_cnt;
			}
			else {
				my_value = value(p_rule->string, 0, 1, 1);
#ifdef DEBUG_REST
				printf("rule[(%d,%d) = %d] = %s left = %d diagonal = %d up = %d my_value = %d\n",
					i, j, (i - 1) * (size - 1) + j - 1, rule[(i - 1) * (size - 1) + j - 1].string, left, 1, 1,
					my_value);
#endif
				if (my_value == 0)
					++zero_cnt;
				else if (my_value == 1) {
					++one_cnt;
				}
				else {
					if (curr_row->undefined) {
						curr_row->two_count = curr_row->undefined - zero_cnt - one_cnt;
						goto row_done;
					}
				}
				left = my_value;
			}
		}
		if (j < size) {
			if (prev_row->one_count == 0) {
				//printf("current_col = %d left = %d diagonal = %d up = %d\n", j, left, 2, 2);
#ifdef DEBUG_REST
				printf("rule[(%d,%d) = %d] = %s left = %d diagonal = %d up = %d\n", i, j, (i - 1) * (size - 1) + j - 1, rule[(i - 1) * (size - 1) + j - 1].string, left, 2, 2);
#endif
				my_value = value(p_rule->string, left, 2, 2);
			}
			else {
#ifdef DEBUG_REST
				printf("rule[(%d,%d) = %d] = %s left = %d diagonal = %d up = %d\n", i, j, (i - 1) * (size - 1) + j - 1, rule[(i - 1) * (size - 1) + j - 1].string, left, 1, 2);
#endif
				my_value = value(p_rule->string, left, 1, 2);
			}
			if (my_value == 0)
				++zero_cnt;
			else if (my_value == 1) {
				++one_cnt;
			}
			else {
				if (curr_row->undefined) {
					curr_row->two_count = curr_row->undefined - zero_cnt - one_cnt;
					goto row_done;
				}
			}
			++j;
			++p_rule;
		}
		for (; j < size; ++j, ++p_rule) {
			left = my_value;
#ifdef DEBUG_REST
			printf("rule[(%d,%d) = %d]=%s left = %d diagonal = %d up = %d\n", i, j, (i - 1) * (size - 1) + j - 1, rule[(i - 1) * (size - 1) + j - 1].string, left, 2, 2);
#endif
			my_value = value(p_rule->string, left, 2, 2);
			if (my_value == 0)
				++zero_cnt;
			else if (my_value == 1) {
				++one_cnt;
			}
			else {
				if (curr_row->undefined) {
					curr_row->two_count = curr_row->undefined - zero_cnt - one_cnt;
					//curr_row->undefined = 0;
					goto row_done;
				}
			}
		}
	row_done:
		if (zero_cnt != 0)
			for (k = i; k < size; ++k) {
				rows[k].zero_count += zero_cnt;
				rows[k].undefined -= zero_cnt;
			}
		curr_row->one_count += one_cnt;
		curr_row->undefined -= one_cnt;
		curr_row->undefined -= curr_row->two_count;
	}
	return 0;
}

void print_row(struct growth_count *row, int format)
{
	//char print_buffer[]
	int i;
	if (format == 1)
		printf("(%d, %d, %d, %d)\n", row->zero_count, row->one_count, row->two_count, row->undefined);
	else {
		for (i = 0; i < row->zero_count; ++i)
			printf("0 ");
		for (i = 0; i < row->one_count; ++i)
			printf("1 ");
		for (i = 0; i < row->two_count; ++i)
			printf("2 ");
		for (i = 0; i < row->undefined; ++i)
			printf("? ");
		printf("\n");
	}
}

void print_rows(struct growth_count *rows, int size, int format)
{
	int i;
	for (i = 0; i < size; ++i) {
		print_row(rows + i, format);
	}
}

void apply_rows(struct hive *hive, struct growth_count *rows)
{
	int i, j, *p;
	p = hive->matrix;
	for (i = 0; i < hive->size; ++i) {
		p += rows[i].zero_count;
		for (j = 0; j < rows[i].one_count; ++j) {
			++*p;
			++p;
		}
		for (j = 0; j < rows[i].two_count; ++j) {
			*p += 2;
			++p;
		}
	}
}

struct hive *solve(struct config *p_config)
{
	int i;
	struct hive *p_result = (struct hive *)malloc(sizeof(struct hive));
	struct growth_count *rows = (struct growth_count *)malloc(sizeof(struct growth_count) * p_config->size);

	revise_rule(p_config->rule, p_config->rule_number);
	p_result->matrix = init_matrix(p_config->matrix_size);
	p_result->size = p_config->size;
	for (i = 0; i < p_config->nDays; ++i) {
		memset(rows, 0, sizeof(struct growth_count) * p_config->size);
		fill_first_col_row(rows, p_config->growth + i, p_config->size);
		//print_rows(rows, p_config->size, 0);
		fill_rest(rows, p_config->rule, p_config->size);
		//printf("%d %d %d\n", p_config->growth[i].zero_count, p_config->growth[i].one_count, p_config->growth[i].two_count);
		//print_rows(rows, p_config->size, 0);
		apply_rows(p_result, rows);
	}
	
	return p_result;
}

int main(void)
{
	freopen("input20.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	struct config config;
	load_config(&config);
	//verify_config(&config);

	struct hive *result = solve(&config);
	print_hive(result);
	return 0;
}
