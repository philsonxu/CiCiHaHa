#include "cllm.h"
#include "model.h"
#include "dataset.h"
#include <string.h>

int main(int argc, char **argv){
    const char *model_path="model.bin", *prompt="\n", *data_path="data/input.txt";
    int n_tokens=500; float temp=0.8f;
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"--model")&&i+1<argc)model_path=argv[++i];
        else if(!strcmp(argv[i],"--prompt")&&i+1<argc)prompt=argv[++i];
        else if(!strcmp(argv[i],"--tokens")&&i+1<argc)n_tokens=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--temp")&&i+1<argc)temp=(float)atof(argv[++i]);
        else if(!strcmp(argv[i],"--data")&&i+1<argc)data_path=argv[++i];
        else if(!strcmp(argv[i],"-h")||!strcmp(argv[i],"--help")){
            printf("Usage: %s --model model.bin --prompt \"ROMEO:\" --tokens 500 --temp 0.8\n",argv[0]);return 0;
        }
    }
    printf("Loading %s ...\n",model_path);
    GPTModel *m = gpt_load(model_path);
    if(!m){fprintf(stderr,"Failed to load model. Train first.\n");return 1;}
    GPTConfig cfg = gpt_config(m);
    printf("Loaded: d=%d heads=%d layers=%d T=%d vocab=%d params=%lld\n",
           cfg.d_model,cfg.n_heads,cfg.n_layers,cfg.block_size,cfg.vocab_size,gpt_param_count(m));
    CharDataset *ds = dataset_load(data_path);
    if(!ds){fprintf(stderr,"dataset load failed\n");gpt_free(m);return 1;}
    int plen=(int)strlen(prompt);
    int T=cfg.block_size;
    if(plen>T-1)plen=T-1;
    int *tokens=(int*)malloc((size_t)(plen+n_tokens+1)*sizeof(int));
    for(int i=0;i<plen;i++){
        int t=ds->stoi[(unsigned char)prompt[i]];
        if(t<0)t=0;
        tokens[i]=t;
    }
    printf("\n--- %d tokens, temp=%.2f ---\n%s",n_tokens,temp,prompt);fflush(stdout);
    gpt_generate(m,tokens,plen,tokens,n_tokens,temp);
    for(int i=plen;i<plen+n_tokens;i++)putchar(ds->itos[tokens[i]]);
    printf("\n");
    free(tokens);dataset_free(ds);gpt_free(m);
    return 0;
}
