#include "PIG.h"
#include "CPigForm.h"
#include "CPigComponente.h"
#include "CPigBotao.h"
#include "CPigCaixaTexto.h"
#include "CPigCampoTexto.h"
#include "CPigCampoSenha.h"
#include "CPigAreaDeTexto.h"
#include "CPigCheckBox.h"

PIG_Evento evento;
PIG_Teclado meuTeclado;

#include <string>

using namespace std;

int x=100;
int y=100;
int xQ = 100;
int yQ = 100;
int larg = 300;
int alt = 300;
int margemHorEsq = 30;
int margemHorDir = 30;
int margemVertCima = 30;
int margemVertBaixo = 30;

int main( int argc, char* args[] ){

    CriaJogo("Meu Jogo");
    
    string texto;
    // char *texto = (char*)malloc(sizeof(char)*100);

    meuTeclado = GetTeclado();

    int fonteTexto = CriaFonteNormal("..//fontes//calibri.ttf", 30, VERMELHO, 0, PRETO);

    CPigCampoSenha *ct = new CPigCampoSenha(9, x, y, alt, larg, "../assets/img/caixa.png", fonteTexto, fonteTexto, 400, false, 0, 0);

    ct->SetPosicaoPadraoLabel(COMPONENTE_ESQUERDA);
    ct->SetLabel("Botao");
    ct->SetTexto("teste");
    ct->SetCorCursor(BRANCO);
    ct->SetMargens(30,30);

    while(JogoRodando()){

        evento = GetEvento();

        ct->TrataEvento(evento);

        IniciaDesenho();

        ct->Desenha();
       
        EncerraDesenho();
    }

    FinalizaJogo();

    return 0;
}

