#include "PIG.h"
#include "CPigForm.h"
#include "CPigComponente.h"
#include "CPigBotao.h"
#include "CPigCaixaTexto.h"
#include "CPigCampoTexto.h"
#include "CPigCampoSenha.h"
#include "CPigAreaDeTexto.h"
#include "CPigCheckBox.h"

PIG_Evento evento;          //evento ser tratadoi a cada pssada do loop principal
PIG_Teclado meuTeclado;     //vari�vel como mapeamento do teclado

int x=50;
int y=50;
int xQ = 100;
int yQ = 100;
int larg = 300;
int alt = 300;
int margemHorEsq = 30;
int margemHorDir = 30;
int margemVertCima = 30;
int margemVertBaixo = 30;

int main( int argc, char* args[] ){

    //criando o jogo (aplica��o)
    CriaJogo("Meu Jogo");
    char *texto = (char*)malloc(sizeof(char)*100);

    //associando o teclado (basta uma �nica vez) com a vari�vel meuTeclado
    meuTeclado = GetTeclado();

     int fonteTexto = CriaFonteNormal("..//fontes//calibri.ttf",30,BRANCO,0,PRETO);
     int fonteLabel = CriaFonteNormal("..//fontes//calibri.ttf",15,VERDE,0,PRETO);

    CPigAreaDeTexto *At = new CPigAreaDeTexto(9,x,y,alt,larg,"caixa.png",fonteTexto,fonteLabel,400,false,0,0,GetTamanhoFonte(fonteTexto)+30,larg - (margemHorEsq + margemHorDir),true,false);


    //CPigCampoSenha *ct = new CPigCampoSenha(9,x,y,alt,larg,"caixa.png",fonteTexto,fonteLabel,400,false,0,0);
/*
    ct->SetPosicaoPadraoLabel(COMPONENTE_ESQUERDA);
    ct->SetLabel("Bot�o");
    ct->SetTexto("teste");
    ct->SetCorCursor(BRANCO);
*/

    At->SetLabel("Bot�o");
    //At->SetFonteLabel(0);
    At->SetTexto("teste");
    At->SetCorCursor(BRANCO);

    At->SetCorLinhasTexto(AZUL);
   // At->SetLargMaxTexto(150);
    At->SetMargens(margemHorEsq,margemHorDir,margemVertBaixo,margemVertCima);
//    At->SetLargMotaxTexto();
    At->SetEspacoEntreAsLinhas(0);


    while(JogoRodando()){

        //pega um evento que tenha ocorrido desde a �ltima passada do loop
        evento = GetEvento();
/*
        if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.tecla==TECLA_CTRLESQUERDO){
            x+=10;
            ct->Move(x,y);
        }
        if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.tecla==TECLA_SHIFTESQUERDO){
            x-=10;
            ct->Move(x,y);
        }
        if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.tecla==TECLA_CTRLDIREITO){
            y+=10;
            ct->Move(x,y);
        }
        if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.tecla==TECLA_SHIFTDIREITO){
            y-=10;
            ct->Move(x,y);
        }
*/


        //ct->TrataEvento(evento);

        At->TrataEvento(evento);

        IniciaDesenho();

        //todas as chamadas de desenho devem ser feitas aqui na ordem desejada
       //ct->Desenha();
        At->Desenha();

        //o frame totalmente pronto ser� mostrado na tela
        EncerraDesenho();
    }

    //o jogo ser� encerrado
    FinalizaJogo();

    return 0;
}

