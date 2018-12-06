/*Projeto para estudo contendo CRUD com Arquivo e Login
	1. Não deixa cadastrar / alterar nomes / logins repetidos
	2. Validação de Login para ADMIN / Gerente / Vendedor
	3. Somente o ADMIN permite pesquisar (consultar) dados completos de todos os usuários
	4. Gerente / Vendedor consultam seus dados individualmente
	5. Id automatico e nao repetido
	6. Cadastra / Altera / Exclui / Consulta / Lista / Pesquisa
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

//estrutura para Login de usuario
typedef struct Usuario{
	char login[20], senha[20];
}Usuario;

//Estrutura para manutenção de funcionario
typedef struct Funcionario{
	char nome[20], endereco[200];
	int id, idade, cargo;
	float salario;
	struct Usuario usu;
}Funcionario;

//Chamada de Procedimentos / Funções
void cadastrar();
void listar();
void alterar();
void excluir();
void pesquisar(char nome[20]);
void consulta(char nome[20]);
void menuCrud(char nome[20]);
void menuVendedor(char nome[20]);
void validaLogin();
int verificaNomeExistente(char nome[20]);

//Ponteiro do Arquivo Funcionario
FILE *pFuncionario;


//Main - Função Principal
int main(){
	validaLogin();
	//menuCrud();
}

//Procedimento - CADASTRAR
void cadastrar(){
	char nome[20];
	int op,cargo;
	
	system("cls");
	printf("\n\n\t--------------------------------------------------\n\t\t\tCADASTRO DE FUNCIONARIOS\n\t--------------------------------------------------\n\n");
	printf("\n Informe os dados do Funcionario.\n\n Nome: ");
	fflush(stdin);
	gets(nome);
	if ((verificaNomeExistente(strupr(nome)))==0){
		Funcionario func;
		func.id = verificaUltimoId();
		pFuncionario = fopen("Funcionarios.txt","ab+");
		strcpy(func.nome,strupr(nome));
		printf(" Endereco: ");
		fflush(stdin);
		gets(func.endereco);
		printf(" Idade: ");
		scanf("%d",&func.idade);
		printf(" Salario R$: ");
		scanf("%f",&func.salario);
		do{
			printf("\n\n Informe o cargo: \n [1] Gerente\n [2] Vendedor\n Opcao: ");
			scanf("%d",&cargo);
			if ((cargo <= 0) || (cargo > 2)){
				printf(" Opcao Invalida!\n");
			}else{
				func.cargo = cargo;
			}
		}while((cargo <= 0) || (cargo > 2));

		printf("\n\n Informe os dados para acesso do sistema.\n\n Login: %s\n",strupr(func.nome));
		strcpy(func.usu.login,strupr(func.nome));
		//fflush(stdin);
		//gets(func.usu.login);
		printf(" Senha: ");
		fflush(stdin);
		gets(func.usu.senha);
		if (fwrite(&func,sizeof(Funcionario),1,pFuncionario) !=1){
			printf("\n Nao foi possivel cadastrar o registro!\n");
		}else{
			printf("\n Novo Funcionario cadastrado com sucesso!\n ");
			fclose(pFuncionario);
		}
	}
	else{
		printf("\n Este Funcionario ja esta cadastrado na base de dados!\n ");	
	}
	fclose(pFuncionario);
	getch();
	system("cls");
}

//Procedimento - LISTAR
void listar(){
	system("cls");
	int verificaCadFun = 0;
	Funcionario auxfunc;
	pFuncionario = fopen("Funcionarios.txt","rb");
	if(pFuncionario!=NULL){
		printf("\n\n\t--------------------------------------------------\n\t\t\tLISTAGEM DE FUNCIONARIOS\n\t--------------------------------------------------\n\n");
		rewind(pFuncionario);
		while(!feof(pFuncionario)){
			fread(&auxfunc,sizeof(Funcionario),1,pFuncionario);
			if(!feof(pFuncionario)){
				printf(" Id: %i\n Nome: %s\n Endereco: %s\n Idade: %d\n",auxfunc.id,auxfunc.nome,auxfunc.endereco,auxfunc.idade);
				if (auxfunc.cargo == 1){
					printf(" Cargo: Gerente\n\n");
				}
				if (auxfunc.cargo == 2){
					printf(" Cargo: Vendedor\n\n");
				}
				verificaCadFun = 1;
			}
		}
		fclose(pFuncionario);
	}else{
		printf(" Erro ao tentar abrir arquivo\n");
	}
	
	if (verificaCadFun == 0){
		printf("\n\t  Nao ha funcionarios cadastrados para listar...\n");
	}
	fclose(pFuncionario);
	getch();
	system("cls");
}

//Procedimento - ALTERAR
void alterar(){
	
	Funcionario func, funcAlt;
	int cont=-1;
	char nomePesquisa[20], nome[20];
	
	system("cls");
	printf("\n\n\t--------------------------------------------------\n\t\t\tALTERAR FUNCIONARIO\n\t--------------------------------------------------\n\n");
	printf("\n Dados da Pesquisa...\n Informe o NOME do Funcionario que deseja alterar:\n ");
	fflush(stdin);
	gets(nomePesquisa);
	if(verificaNomeExistente(strupr(nomePesquisa)) == 1){
		printf("\n\n Dados do Funcionario localizado:\n");
		consulta(strupr(nomePesquisa));
		printf("\n Informe os dados para alteracao do Funcionario.\n");
		printf(" Nome: ");
		fflush(stdin);
		gets(nome);
		if((verificaNomeExistente(strupr(nome)) == 0) || ((verificaNomeExistente(strupr(nome)) == 1) && (strcmp(nomePesquisa,strupr(nome)) == 0))){
			pFuncionario = fopen("Funcionarios.txt","rb+");
			rewind(pFuncionario);	
			while(!feof(pFuncionario)){
				fread(&func,sizeof(Funcionario),1,pFuncionario);
				if(!feof(pFuncionario)){
					cont++;
					if(strcmp(func.nome,strupr(nomePesquisa))==0){
						strcpy(funcAlt.nome,strupr(nome));
						printf(" Endereco: ");
						fflush(stdin);
						gets(funcAlt.endereco);
						printf(" Idade: ");
						scanf("%d",&funcAlt.idade);
						printf(" Valor Salario R$: ");
						scanf("%f",&funcAlt.salario);
						printf(" Login: %s\n",func.usu.login);
						//fflush(stdin);
						//gets(funcAlt.usu.login);
						printf(" Senha: ");
						fflush(stdin);
						gets(funcAlt.usu.senha);
						strcpy(funcAlt.usu.login,strupr(func.usu.login));
						funcAlt.id = func.id;
						funcAlt.cargo = func.cargo; 
						fseek(pFuncionario,cont*sizeof(Funcionario),SEEK_SET);
						fwrite(&funcAlt,sizeof(Funcionario),1,pFuncionario);
						fclose(pFuncionario);
						printf("\n Dados do Funcionario alterado com sucesso!\n");	
						fclose(pFuncionario);
						break;
					}
				}
			}
		}
		if ((verificaNomeExistente(nomePesquisa) == 1) && (strcmp(func.nome,nomePesquisa) != 0)){
			printf("\n Nao foi possivel alterar os dados do Funcionario.\n Voce esta tentando inserir um nome que ja existe na base de dados.\n ");		
		}
	}
	else{
		printf("\n Funcionario nao localizado!\n ");	
	}
	fclose(pFuncionario);
	getch();
	system("cls");
}

//Procedimento - EXCLUIR
void excluir(){
	char nomePesquisa[20];
	Funcionario funcExcluir;
	
	FILE *pExcluir;
	
	system("cls");
	printf("\n\n\t--------------------------------------------------\n\t\t\tENCLUIR FUNCIONARIO\n\t--------------------------------------------------\n\n");
	printf("\n Dados da Pesquisa...\n Informe o NOME do Funcionario que deseja excluir:\n ");
	fflush(stdin);
	gets(nomePesquisa);
	if(verificaNomeExistente(strupr(nomePesquisa))== 1){
		fclose(pFuncionario);
		
		pFuncionario = fopen("Funcionarios.txt","rb+");
		
		pExcluir = fopen("pExcluir.txt","wb+");
		
		rewind(pFuncionario);
		while(!feof(pFuncionario)){
			fread(&funcExcluir,sizeof(Funcionario),1,pFuncionario);
			if(!feof(pFuncionario)){
				if(strcmp(funcExcluir.nome,strupr(nomePesquisa))==0){
					printf("\n Funcionario Localizado!\n Registro excluido com sucesso!\n ");
				}else{
					fwrite(&funcExcluir,sizeof(Funcionario),1,pExcluir);
				}
			}
		}
		fclose(pFuncionario);
		fclose(pExcluir);
		remove("Funcionarios.txt");
		rename("pExcluir.txt","Funcionarios.txt");
	}else{
		printf("\n Funcionario nao localizado!\n ");		
	}

	getch();
	system("cls");
}

//Procedimento - CONSULTA um registro por nome do funcionario com passagem de parametro
void consulta(char nome[20]){
	Funcionario auxfunc;
	pFuncionario = fopen("Funcionarios.txt","rb");
	if(pFuncionario!=NULL){
		rewind(pFuncionario);
		while(!feof(pFuncionario)){
			fread(&auxfunc,sizeof(Funcionario),1,pFuncionario);
			if(!feof(pFuncionario)){
				if(strcmp(nome,auxfunc.nome)==0){
					printf(" Id: %i \n Nome: %s\n Endereco: %s\n Idade: %d\n Valor Salario R$: %.2f\n Login: %s\n Senha: %s\n",auxfunc.id,auxfunc.nome,auxfunc.endereco,auxfunc.idade,auxfunc.salario,auxfunc.usu.login,auxfunc.usu.senha);
					if (auxfunc.cargo == 1){
						printf(" Cargo: Gerente\n\n");
					}
					if (auxfunc.cargo == 2){
						printf(" Cargo: Vendedor\n\n");
					}
				}
			}
		}
		fclose(pFuncionario);
	}else{
		printf(" Erro ao tentar abrir arquivo\n");
	}
	
	fclose(pFuncionario);

}

//Procedimento - PESQUISAR dados do funcionario (Somente o ADMIN tem acesso a dados completo de todos os funcionarios)
void pesquisar(char nomeUsuario[20]){
	system("cls");
	char nomePesquisa[20];
	if (strcmp(nomeUsuario, "ADMIN") == 0){
		printf("\n\n\t--------------------------------------------------\n\t\t\tPESQUISA DE FUNCIONARIOS\n\t--------------------------------------------------\n\n");
		printf("\n Dados da Pesquisa...\n Informe o NOME do Funcionario que deseja localizar:\n ");
		fflush(stdin);
		gets(nomePesquisa);
		if (verificaNomeExistente(strupr(nomePesquisa)) == 1){
			printf("\n Dados do Funcionario localizado:\n");
			consulta(strupr(nomePesquisa));
		}else{
			printf("\n Funcionario nao localizado!\n ");
		}
	}else{
		printf("\n\n\t--------------------------------------------------\n\t\t\tDADOS DO FUNCIONARIO\n\t--------------------------------------------------\n\n");
		consulta(nomeUsuario);
	}
	getch();
	system("cls");
}

//Função que verifica se um nome ja esta cadastrado e nao permite que o usuario cadastre novamente o mesmo nome
int verificaNomeExistente(char nome[20]){
	Funcionario aux;

	int verificador = 0; // 1 - cadastrado / 0 - Nao cadastrado
	
	pFuncionario = fopen("Funcionarios.txt","rb");
	if(pFuncionario!=NULL){
		rewind(pFuncionario);
		while(!feof(pFuncionario)){
			
			fread(&aux,sizeof(Funcionario),1,pFuncionario);
			if(!feof(pFuncionario)){
				if(strcmp(aux.nome,nome)==0){
					verificador = 1;
				}
			}
		}
	}else{
		printf("\n Erro ao abrir arquivo.\n");	
	}
	fclose(pFuncionario);
	return verificador;
}

//Função verificadora do ultimo ID gerado
int verificaUltimoId(){
	Funcionario aux;

	int contador = 0, verificaId; 
	
	
	pFuncionario = fopen("Funcionarios.txt","rb");
	if(pFuncionario!=NULL){
		rewind(pFuncionario);
		while(!feof(pFuncionario)){
			fread(&aux,sizeof(Funcionario),1,pFuncionario);
			if(!feof(pFuncionario)){
				contador++;
				verificaId = aux.id;
			}
		}
	}else{
		printf("\n Erro ao abrir arquivo.\n");	
	}
	fclose(pFuncionario);
	
	if (contador == 0){
		return contador + 1;	
	}
	
	if (verificaId >= contador){
		return verificaId + 1;	
	}
	
	
}

//Procedimento de Validação de Login Admin ou Gerente / Vendedor / ADMIN
void validaLogin(){
	char login[20],senha[20];
	int i=0, verificaLogin = 0;
	
	do{
		system("cls");
		printf("\n\n\n\n\n\n\n\t\t\t\t\t----------------------------------------------\n\t\t\t\t\t\t\tBEM VINDO AO SISTEMA\n\t\t\t\t\t----------------------------------------------\n\n");
		printf("\t\t\t\t\t\t   Informe os Dados de Acesso\n\n\n");
		printf("\t\t\t\t\t\t\tLogin: ");
		fflush(stdin);
		gets(login);
		printf("\n\t\t\t\t\t\t\tSenha: ");
		fflush(stdin);
		gets(senha);
		
		if(strcmp(strupr(login), "ADMIN") == 0){
			
			if (strcmp(senha, "123") == 0){
				verificaLogin = 1;
				menuCrud("ADMIN");
			}else{
				verificaLogin = 0;
				printf("\n\t\t\t\t\t\t\tSenha incorreta!\n\t\t\t\t\t");
				getch();
				validaLogin();	
			}
		}
		
		if(strcmp(strupr(login), "ADMIN") != 0){
			Funcionario aux;
			pFuncionario = fopen("Funcionarios.txt","rb");
			if(pFuncionario!=NULL){
				rewind(pFuncionario);
				while(!feof(pFuncionario)){
					fread(&aux,sizeof(Funcionario),1,pFuncionario);
					if(!feof(pFuncionario)){
						if(strcmp(strupr(aux.usu.login),strupr(login))==0){
							if(strcmp(strupr(aux.usu.senha),strupr(senha))==0){
								verificaLogin = 1;
								break;
							}
						}
					}
				}
				if (verificaLogin == 1){
					if (aux.cargo == 1){
						fclose(pFuncionario);
						menuCrud(strupr(login));
					}
					if (aux.cargo == 2){
						fclose(pFuncionario);
						menuVendedor(strupr(login));
					}
				}
			}
			if (verificaLogin == 0){
				printf("\n\t\t\t\t\t\t    Login ou senha incorretos!\n\t\t\t\t\t");
				getch();
			}
		}	
		
	}while(verificaLogin !=1);
	
}

//Procedimento MENU contendo o CRUD da aplocação (acessado apenas pelos usuarios ADMIN ou um usuario GERENTE)
void menuCrud(char nome[20]){
	int op;

	do{
		system("cls");
		printf("\n\n\n\n\n\t\t\t\t--------------------------------------------------\n\t\t\t\tM E N U    D E  O P C O E S    D O   S I S T E M A\n\t\t\t\t--------------------------------------------------\n\n\n");
		if (strcmp(nome, "ADMIN") == 0){
			printf("\t\t\t\t\t     CADASTRO DE FUNCIONARIOS\n\n\t\t\t\t\tSelecione uma das Opcoes abaixo...\n\n\t\t\t\t\t\t[1] CADASTRAR\n\t\t\t\t\t\t[2] LISTAR \n\t\t\t\t\t\t[3] ALTERAR \n\t\t\t\t\t\t[4] EXCLUIR\n\t\t\t\t\t\t[5] CONSULTAR\n\t\t\t\t\t\t[6] REFAZER LOGIN\n\t\t\t\t\t\t[0] SAIR\n\n\t\t\t\t\t\tOPCAO: ");
		}
		else{
			printf("\t\t\t\t\t     CADASTRO DE FUNCIONARIOS\n\n\t\t\t\t\tSelecione uma das Opcoes abaixo...\n\n\t\t\t\t\t\t[1] CADASTRAR\n\t\t\t\t\t\t[2] LISTAR \n\t\t\t\t\t\t[3] ALTERAR \n\t\t\t\t\t\t[4] EXCLUIR\n\t\t\t\t\t\t[5] SEUS DADOS\n\t\t\t\t\t\t[6] REFAZER LOGIN\n\t\t\t\t\t\t[0] SAIR\n\n\t\t\t\t\t\tOPCAO: ");
		}
		scanf("%d",&op);
		switch(op){
			case 1:
				cadastrar();
				menuCrud(nome);
				break;
			case 2:
				listar();
				menuCrud(nome);
				break;
			case 3:
				alterar();
				menuCrud(nome);
				break;
			case 4: 
				excluir();
				menuCrud(nome);
				break;
			case 5:
				pesquisar(nome);
				menuCrud(nome);
				break;
			case 6:
				main();
				break;	
			case 0:
				exit(0);
				break;
			
		}
		if ((op < 0) || (op > 6)){
	        printf("\n\n\t\t\t\t\t\tOpcao Invalida!\n ");
	        getch();
		}
	}while(op !=0);
	system("cls");
	getch();
}


//Procedimento MENU Vendedor
void menuVendedor(char nome[20]){
	int op;

	do{
		system("cls");
		printf("\n\n\n\n\n\n\t\t\t\t--------------------------------------------------\n\t\t\t\tM E N U    D E  O P C O E S    D O   S I S T E M A\n\t\t\t\t--------------------------------------------------\n\n\n");
		printf("\t\t\t\t\tSelecione uma das Opcoes abaixo...\n\n\t\t\t\t\t\t[1] LISTAR FUNCIONARIOS\n\t\t\t\t\t\t[2] SEUS DADOS \n\t\t\t\t\t\t[3] REFAZER LOGIN \n\t\t\t\t\t\t[0] SAIR\n\n\t\t\t\t\t\tOPCAO: ");
		
		scanf("%d",&op);
		switch(op){
			case 1:
				listar();
				menuVendedor(strupr(nome));
				break;
			case 2:
				pesquisar(nome);
				menuVendedor(strupr(nome));
				break;
			case 3:
				main();
				break;	
			case 0:
				exit(0);
				break;
			
		}
		if ((op < 0) || (op > 2)){
	        printf("\n\n\t\t\t\t\t\tOpcao Invalida!\n ");
	        getch();
		}
	}while(op !=0);
	system("cls");
	getch();
}
