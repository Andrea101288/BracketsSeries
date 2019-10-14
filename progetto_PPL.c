/*********************************************************************/
/* programma che restituisce prefisso parentesi chiuse correttamente */
/*********************************************************************/


/*****************************/
/* inclusione delle librerie */
/*****************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**********************************/
/* definizione nuove di strutture */
/**********************************/


typedef struct elem_lista
{
    char carattere;
    struct elem_lista *succ_p;
} elem_lista_t;


/********************************/
/* dichiarazione delle funzioni */
/********************************/


int acquisizione_dim_stringa_in(void);
void acquisizione_parentesi(char parentesi[],
                            int dim);
int validazione_input(char parentesi[],
                      int num_elem);
void stampa_stringa_corretta(char parentesi[],
			     			 int dim);
void metti_su_pila(elem_lista_t **cima_p,
		   		   char carattere);
elem_lista_t *togli_da_pila(elem_lista_t **cima_p);


/******************************/
/* definizione delle funzioni */
/******************************/


/*definizione della funzione main*/
int main (void)
{

    /*dichiarazione variabili locali alla funzione*/
    char *parentesi;	/* input: stringa input dichiarata come puntatore */
    int   dim;		    /* input: variabile contenente la dimensione
                                  massima della stringa di input */
   
    /*chiamata a funzione: acquisizione dimensione stringa */
    dim = acquisizione_dim_stringa_in(); 
   
    /* allocazione dinamica stringa */
    parentesi = (char *)malloc(dim * sizeof(char));

    /* chiamata a funzione: acquisizione parentesi */
    acquisizione_parentesi(parentesi,
                           dim);
    printf("\nil prefisso è':\n");
	
    /* chiamata a funzione: stampa della stringa controllata */
    stampa_stringa_corretta(parentesi, 
                            dim);

    /* libero la memoria dell'array allocato dinamicamente */
    free(parentesi);							
    return(0);
}

/* definizione della funzione per acquisizione della dimensione
   della stringa di input */
int acquisizione_dim_stringa_in(void)
{

    /*dichiarazione variabili locali alla funzione*/
    int dim;         		/* input: variabile contenente la dimensione massima
                                      della stringa di input */
    char carattere_rimosso; /* lavoro: variabile per pulizia buffer */
    int esito_lettura;      /* lavoro: variabile di controllo acquisizione
                                       tramite funzione scanf */

    /* acquisizione dimensione stringa da allocare */
    do
    {
        /* acquisizione dimensione stringa */
        printf("\n\nDigita numero di parentesi che si vogliono inserire (> 0): ");
        esito_lettura = scanf("%d",
                              &dim);

        /* validazione input */
        if ((esito_lettura != 1) ||
			(dim <= 0))
        {
            printf("\nInput non accettabile!");
            do
                 carattere_rimosso = getchar();
            while (carattere_rimosso != '\n');
        }     
    }
    while ((esito_lettura != 1) ||
		   (dim <= 0));

    /* pulizia buffer */
    while (getchar() != '\n');

    /* valore di ritorno alla funzione */
    return(dim);
}   

/* definizione della funzione per acquisizione parentesi */
void acquisizione_parentesi(char parentesi[],
                            int dim)
{

    /*dichiarazione variabili locali alla funzione*/
    int i,  			    /* lavoro: variabile contatore */
    	valore;				/* lavoro: valore restituito dalla funzione
                           			   validazione_input */
    char c,     			/* input: variabile per acquisizione caratteri con getchar */
		 carattere_rimosso;	/* lavoro: variabile per pulizia buffer */ 

    /* acquisizione e validazione stringa */
    do
    {
        /* Reinizializzo variabili */
        i = 0;

        /* acquisizione delle parentesi (aperte e chiuse) */
		printf("\nDigita sequenza di parentesi (aperte e chiuse):\n");

        do 
		{
			c = getchar();
			parentesi[i] = c;
			i++;
		}
		while(c != '\n' && i < dim);
		
        /* chiamata a funzione: validazione caratteri acquisiti */
        valore = validazione_input(parentesi,
                                   i);
        /* validazione input */
        if(valore != 1) 
		{
            /* se la stringa era più lunga della dimensione allocata
			   i caratteri in eccesso vengono eliminati dal buffer */
            if(i == dim)
			{
				do
					carattere_rimosso = getchar();
				while(carattere_rimosso != '\n');
			}            
		    printf("\nInput non accettabile reinserire parentesi!\n\n");
        }
    }
	while(valore != 1);
}

/* definizione della funzione per la validazione caratteri acquisiti */
int validazione_input(char parentesi[],
                      int num_elem) 
{

    /* dichiarazione variabili locali alla funzione */
    int i = 0,      /* lavoro: variabile contatore */
        valore = 0; /* lavoro: valore restituito alla funzione */

    /* lettura e validazione dei caratteri acquisiti */
    for (i = 0;
         (i < num_elem);
         i++)
    {
        if ((parentesi[i] != '(') &&
            (parentesi[i] != '[') &&
            (parentesi[i] != '{') &&
            (parentesi[i] != ')') &&
            (parentesi[i] != ']') &&
            (parentesi[i] != '}') &&
		    (parentesi[i] != '\n'))
        {
            valore = 0;
            i = num_elem;
        }
        else valore = 1;
    }

    /* lettura e validazione se presente carattere spaziatura */
    for (i = 0;
         (i < num_elem);
         i++)
    {
        if (parentesi[i] == ' ')
            valore = 0;
    }

    /* valore restituito alla funzione.
	   0 se input errato.
       1 se input corretto. */
    return(valore);
}

/* definizione della funzione per stampare la sequenza corretta di parentesi */
void stampa_stringa_corretta(char parentesi[],
			     			 int dim)
{
    /* dichiarazione di variabili locali alla funzione */
    elem_lista_t *cima = NULL; 	/* lavoro: variabile che indica l'elemento in cima alla pila */
    elem_lista_t *tmp;	 		/* lavoro: variabile temporanea di appoggio per lettura carattere */ 
    int i = 0;	         		/* lavoro: variabile contatore */
	
    /* finchè l'elemento è una parentesi aperta la inserisco nella pila, appena incontro una parentesi chiusa la controllo
       con la cima della pila. Se corrispondono le due parentesi le tolgo dalla pila */
    for (i = 0;
		 (i < dim);
		 i++)
    {
		/* Fino a quando l'input è una parentesi aperta viene inserita nella pila e stampata a video */
        if ((parentesi[i] == '(') ||
            (parentesi[i] == '[') ||
	    	(parentesi[i] == '{'))
        {
            metti_su_pila(&cima, 
                          parentesi[i]);
            printf("%c", 
                   parentesi[i]);
        }
        else
        {
            /* variabile temporanea per il confronto quando si incontra una parentesi chiusa */
            tmp = togli_da_pila(&cima);
			
            /* esco dal ciclo se la pila e' vuota */
            if (tmp == NULL)
            	i = dim;
			
			/* se in cima alla pila si trova una parentesi aperta che viene chiusa correttamente,
			   allora la tolgo dalla pila, la stampo e libero la memoria della variabile temporanea */
            else if (((tmp->carattere == '(') && (parentesi[i] == ')')) ||
                     ((tmp->carattere == '[') && (parentesi[i] == ']')) ||
                     ((tmp->carattere == '{') && (parentesi[i] == '}')))
            {
            	printf("%c",
					   parentesi[i]);
	        	free(tmp);
            }
            else
            {
                i = dim;
                printf("\n");
            }  
        }
    }
	printf("\n");
}

/* definizione della funzione per mettere un elemento in cima alla pila */
void metti_su_pila(elem_lista_t **cima_p,
		   		   char carattere)
{
    /* dichiarazione di variabili locali alla funzione */
    elem_lista_t *nuovo_p;

    nuovo_p = (elem_lista_t *)malloc(sizeof(elem_lista_t));
    nuovo_p->carattere = carattere;
    nuovo_p->succ_p = *cima_p;
    *cima_p = nuovo_p;
}

/* definizione della funzione per togliere l'elemento in cima alla pila */
elem_lista_t *togli_da_pila(elem_lista_t **cima_p)
{
    /* dichiarazione di variabili locali alla funzione */
    elem_lista_t *elem_p;
    
	elem_p = *cima_p;
    if(*cima_p != NULL)
        *cima_p = (*cima_p)->succ_p;
	else
		*cima_p = NULL;

    return elem_p;
}
