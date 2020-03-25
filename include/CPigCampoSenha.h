
class CPigCampoSenha: public CPigCaixaTexto{

private:

int margemHor,margemVert;
char mascara;

void DesenhaLabel(){

    if (strcmp(label,""))
        int px,py;
        switch(posLabel){
        case COMPONENTE_CIMA:
            EscreverCentralizada(label,x+larg/2,y+alt+5,fonteLabel);
            break;
        case COMPONENTE_CIMADIR:
            EscreverDireita(label,x+larg,y+alt+5,fonteLabel);
            break;
        case COMPONENTE_CIMAESQ:
            EscreverEsquerda(label,x,y+alt+5,fonteLabel);
            break;
        case COMPONENTE_BAIXO:
            EscreverCentralizada(label,x+larg/2,y-30,fonteLabel);
            break;
        case COMPONENTE_BAIXODIR:
            EscreverDireita(label,x+larg,y-30,fonteLabel);
            break;
        case COMPONENTE_BAIXOESQ:
            EscreverEsquerda(label,x,y-30,fonteLabel);
            break;
        case COMPONENTE_ESQUERDA:
            EscreverDireita(label,x-5,y+margemVert,fonteLabel);
            break;
        case COMPONENTE_DIREITA:
            EscreverEsquerda(label,x+larg+5,y+margemVert,fonteLabel);
            break;
        case COMPONENTE_POSICAO_PERSONALIZADA:
            EscreverEsquerda(label,x+labelX,y+labelY,fonteLabel);
        }


}

void DesenhaCursor(){

    if (estado==COMPONENTE_EDITANDO){
        if (cursorExibido){

            DesenhaLinhaSimples(xCursor,yCursor,xCursor,yCursor+GetTamanhoFonte(fonteTexto),corCursor,idJanela);


        }
        if (timer&&timer->GetTempoDecorrido()>1){
            cursorExibido = !cursorExibido;
            timer->Reinicia(false);
        }
    }

}

void AjustaBaseTexto(int largParcial){

        while(xCursor>x+larg-margemHor){

            xBase-=5;
            xCursor = xBase + largParcial;

        }

        while(xCursor < x+margemHor){

            xBase+=5;
            xCursor = xBase + largParcial;

        }

}

void AjustaAlinhamento(){
std::string textoBase = GetTextoMask();
std::string aux;

    xBase = x+margemHor;

    aux.assign(textoBase,0,posCursor);
    xCursor = xBase + CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

    AjustaBaseTexto(CalculaLarguraPixels((char*)aux.c_str(),fonteTexto));


}

void EscreveTexto(){

    EscreverEsquerda((char*)GetTextoMask().c_str(),xBase,yBase,fonteTexto);

}

int TrataEventoMouse(PIG_Evento evento){
    std::string textoBase = GetTextoMask();

    if (evento.mouse.acao!=MOUSE_PRESSIONADO) return 0;
    SDL_Point p;
    CMouse::PegaXY(p.x,p.y);
    SDL_Rect r = {x+margemHor,y,larg-(2*margemHor),alt};

    if (SDL_PointInRect(&p,&r)){
        int delta = p.x-xBase;
        int largParcial = 0;
        int largUltimaLetra = 0;

        for (int i=0;i<=textoBase.size();i++){
            std::string aux;

            aux.assign(textoBase,0,i);

            largParcial += CalculaLarguraPixels((char*)aux.c_str(),fonteTexto);

            if (delta<largParcial-largUltimaLetra){
                posCursor = i-1;
                AjustaAlinhamento();
                return 1;
            }

            largUltimaLetra = largParcial;
        }

        posCursor = strlen(texto);

        AjustaAlinhamento();
        if (estado==COMPONENTE_NORMAL)
            DefineEstado(COMPONENTE_EDITANDO);

        return 1;
    }
    return 0;
}

std::string GetTextoMask(){
    std::string resp;
    for (int i=0;i<strlen(texto);i++)
        resp += mascara;

    return resp;
}

public:

CPigCampoSenha(int idComponente,int px, int py, int alt,int larg,char *nomeArq,int fonteDoTexto = 0,int fonteDoLabel = 0,int maxCars = 200, bool apenasNumeros=false, int retiraFundo=1,int janela=0):CPigCaixaTexto(idComponente,px,py,alt,larg,nomeArq,fonteDoTexto,fonteDoLabel,maxCars,apenasNumeros,retiraFundo,janela){

        yBase = y+margemVert;
        xBase = x+margemHor;
        xCursor = xBase;
        yCursor = yBase;
        margemHor= margemVert = 0;
        mascara = '*';

}

~CPigCampoSenha(){
    if (timer) delete timer;
}

int Desenha(){
    //imagem de fundo
    SDL_RenderCopyEx(renderer, text, &frame,&dest,-angulo,&pivoRelativo,flip);

    //SDL_Rect r={x+margemHor-2,altJanela-y-alt,larg-2*(margemHor-2),alt};
    SDL_Rect r={x+margemHor,altJanela-y-alt+margemVert,larg-(2*margemHor),alt-(2*margemVert)+1};
    SDL_RenderSetClipRect(renderer,&r);

    EscreveTexto();
    DesenhaCursor();

    //desbloqueia o desenho fora da area do componente
    SDL_RenderSetClipRect(renderer,NULL);

    DesenhaLabel();

}

int TrataEvento(PIG_Evento evento){

    if (evento.tipoEvento==EVENTO_MOUSE){
        return TrataEventoMouse(evento);
    }else if (evento.tipoEvento==EVENTO_TECLADO){
        return TrataEventoTeclado(evento);
    }

}

};

typedef CPigCampoSenha *PigCampoSenha;
