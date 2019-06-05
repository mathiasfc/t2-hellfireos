Para essa prática, é necessário que você configure o ambiente (compilador e simulador) e veja os exemplos de uso da API em /app (noc_test, noc_test2, noc_test3). Se a imagem da máquina virtual Linux for usada, não é necessário configurar o compilador.

Para configurar o ambiente:

Baixe o compilador cruzado pré construído (GCC 4.6.1 pre-built, 64 bit) do Moodle;
Extraia o arquivo e configure a variável de ambiente PATH (em ~/Downloads)
## $ tar -zxvf gcc-4.6.1.tar.gz
## $ export PATH=$PATH:~/Downloads/gcc-4.6.1/bin
Para tornar a configuração permanente (e disponível em qualquer novo terminal aberto), atualize o script .bashrc no seu diretório home
## $ echo "export PATH=$PATH:~/Downloads/gcc-4.6.1/bin" >> ~/.bashrc
Teste o compilador cruzado com o comando
## $ mips-elf-gcc -v
Alternativamente você pode montar um compilador cruzado no seu ambiente (GCC 4.9.3) com o script fornecido ou utilizar a ISO fornecida com o ambiente já configurado.

Baixe os fontes do HellfireOS do Github. O repositório pode ser clonado com:

## $ git clone https://github.com/sjohann81/hellfireos
## $ cd hellfireos/
Compile os fontes do simulador do ambiente MPSoC (mpsoc_sim) com o compilador nativo da máquina local. Isso será necessário na primeira vez que o ambiente for montado. No exemplo abaixo, estamos configurando um ambiente multiprocessado com 6 processadores, organizados em uma NoC com 2 linhas e 3 colunas. 

## $ cd usr/sim/mpsoc_sim
## $ make noc_3x2
## $ cd ../../../
Verifique o makefile do simulador para outras possíveis configurações. Lembrete: as configurações do kernel devem ser iguais ao simulador! Portanto, se o kernel for configurado para usar, por exemplo, 9 processadores, será necessário recompilar o simulador para uma nova topologia.

Para este exercício, é necessário que você (no diretório do HellfireOS):

Escolha o diretório da plataforma correta
## $ cd platform/noc_3x2
Modifique a o makefile da plataforma, configurando o para a aplicação correta (no primeiro exemplo, use APP = app/noc_test)
Dentro do diretório platform/noc_3x2, compile a aplicação
## $ make images
O processo irá gerar 6 imagens, contendo um kernel por processador e a aplicação. Copie as imagens para o diretório correto do simulador.
Se o diretório usr/sim/mpsoc_sim/objects não existir, crie-o agora: $ mkdir ../../usr/sim/mpsoc_sim/objects
## $ cp *.bin ../../usr/sim/mpsoc_sim/objects
É recomendado que um outro terminal seja aberto para executar a simulação. Dessa forma, o primeiro terminal pode ser usado para configurar o kernel e compilar a aplicação e o segundo terminal pode ser usado para executar a aplicação, visualizar o resultado da simulação e eventualmente modificar a configuração do simulador. Para simular a aplicação por 1 segundo (tempo do hardware):
##  $ cd ../../usr/sim/mpsoc_sim
##  $ ./mpsoc_sim 1 s
No diretório /usr/sim/mpsoc_sim/reports serão criados arquivos out0.txt .. out5.txt contendo a saída do terminal de cada processador simulado.
Realize a modificação do makefile da plataforma para configurar e testar todas as aplicações (noc_test, noc_test2, noc_test3, noc_test4). Verifique os resultados da simulação.

Verifique a documentação sobre a API do HellfireOS que implementa as primitivas para MPSoCs (disponível em https://github.com/sjohann81/hellfireos/blob/master/usr/doc/doxygen.tar.gz)