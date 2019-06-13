#include <hellfire.h>
#include <stdbool.h>
#include "labyrinth.h"

void show(int *m, int lin, int col)
{
	int i, j;

	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			printf("%3d ", m[i * col + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void mark(int *m, int l, int c, int lin, int col)
{
	int h, i, j, k = 1;

	for (h = 0; h < lin * col; h++, k++) {
		for (i = 0; i < lin; i++) {
			for (j = 0; j < col; j++) {
				if (m[i * col + j] == k) {
					if (i - 1 >= 0) {
						if (m[(i - 1) * col + j] == 0)
							m[(i - 1) * col + j] = k + 1;
					}
					if (i + 1 < lin) {
						if (m[(i + 1) * col + j] == 0)
							m[(i + 1) * col + j] = k + 1;
					}
					if (j - 1 >= 0) {
						if (m[i * col + (j - 1)] == 0)
							m[i * col + (j - 1)] = k + 1;
					}
					if (j + 1 < col) {
						if (m[i * col + (j + 1)] == 0)
							m[i * col + (j + 1)] = k + 1;
					}
				}
			}
		}
	}
}

int search(int *m, int i, int j, int ei, int ej, int lin, int col)
{
	int k = 2;

	while (k > 1) {
		k = m[i * col + j];
		printf("[%d,%d] ", i, j);
		if (i - 1 >= 0) {
			if (m[(i - 1) * col + j] < k && m[(i - 1) * col + j] > 0) {
				i--;
				continue;
			}
		}
		if (i + 1 < lin) {
			if (m[(i + 1) * col + j] < k && m[(i + 1) * col + j] > 0) {
				i++;
				continue;
			}
		}
		if (j - 1 >= 0) {
			if (m[i * col + (j - 1)] < k && m[i * col + (j - 1)] > 0) {
				j--;
				continue;
			}
		}
		if (j + 1 < col) {
			if (m[i * col + (j + 1)] < k && m[i * col + (j + 1)] > 0) {
				j++;
				continue;
			}
		}
	}
	if (i == ei && j == ej)
		return 1;
	else
		return 0;
}

int solve(int *m, int lin, int col, int si, int sj, int ei, int ej)
{
	m[ei * col + ej] = 1;
	mark(m, ei, ej, lin, col);
	/* show(m, lin, col); */
	return search(m, si, sj, ei, ej, lin, col);
}

// task_master() eh usada pelo master para enviar mazes aos slaves
void task_master(void){
	struct maze_s *m;
	int i = 0;
	// Variaveis para receber dados dos slaves
	int8_t cpu_slave[4];
	int16_t size_cpu_slave;
	int16_t port_slave;
	int16_t my_cpu, my_port;
	int8_t buf[512];

	printf("\n\n----CPU[%d] - MASTER----\n\n", hf_cpuid());

	// Cria a id e porta da thread master
	hf_comm_create(0, 10000, 0);

	// Itera pelos mazes
	for (i = 0; i < sizeof(mazes) / sizeof(struct maze_s); i++){

		// Espera um slave enviar uma msg dizendo que esta livre
		hf_recv(&my_cpu, &my_port, buf, &size_cpu_slave, 0);

		printf("\nCPU recebida do escravo = %s", buf);
		// Monta a porta conforme CPU recebida
		port_slave = 10000 + cpu_slave;

		printf("\nCPU SLAVE: %d\n PORT SLAVE: %d\n\n", cpu_slave, port_slave);

		// Pega o proximo maze a ser resolvido
		m = &mazes[i];

		// Envia ao slave um maze para ser resolvido
		hf_send(cpu_slave, port_slave, m, sizeof(m), 0);

		printf("\nMaze %d enviado ao CPU[%d]", i, cpu_slave);
	}

	for(;;);
}

// task() eh usada pelos slaves para resolver mazes
void task(void){
	struct maze_s *m;
	int8_t buf[512];
	uint16_t size;
	int i, s, k = 0;
	int16_t cpu_id;
	int16_t my_cpu, my_port;
	int8_t msg[4]; //montando assim pra teste

	// Monta CPU
	cpu_id = hf_cpuid();

	printf("\n\n----CPU[%d] - MAZE SOLVER----\n\n", cpu_id);

	// Monta a msg, que sera sempre a CPU_ID do escravo
	strcpy(buf, "hello world!");


	// Cria a id e porta da thread slave
	hf_comm_create(cpu_id, (10000 + cpu_id), 0);

	m = (struct mase_s *)&buf;

	// Itera infinitamente
	while(1){

		// Envia ao master uma msg dizendo que esta livre
		hf_send(0, 10000, buf, strlen(buf) + 1, 0);

		// Espera retorno do master com o maze
		hf_recv(&my_cpu, &my_port, (int8_t *)m, &size, 0);

		// Resolve o maze
		s = solve(m->maze, m->lines, m->columns, m->start_line, m->start_col, m->end_line, m->end_col);

		if (s) {
			printf("\nOK! Maze resolvido!\n");
			k++;
		} else {
			printf("ERRO!\n");
		}		
	}
	printf("\nsummary: %d mazes were solved\n", k);
}

void app_main(void){
	if(hf_cpuid() == 0){
		hf_spawn(task_master, 0, 0, 0, "Master send mazes", 6000);
	}else if (hf_cpuid() == 1) {
		hf_spawn(task, 0, 0, 0, "Slaves solve mazes", 6000);
	}
}
