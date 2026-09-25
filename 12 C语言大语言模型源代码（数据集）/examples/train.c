#include "cllm.h"
#include "model.h"
#include "dataset.h"

static void usage(const char*p){
    printf("Usage: %s [options]\n",p);
    printf("  --data <path>      text file (default data/input.txt)\n");
    printf("  --out <path>       model output (model.bin)\n");
    printf("  --iters <n>        iterations (2000)\n");
    printf("  --lr <f>           learning rate (5e-4)\n");
    printf("  --d_model/--n_heads/--n_layers/--block_size/--batch_size\n");
    printf("  --gen_every <n>    generate every N steps (200)\n");
    printf("  --gen_tokens <n>   tokens per sample (200)\n");
}

int main(int argc, char **argv) {
    const char *data_path="data/input.txt", *out_path="model.bin";
    int iters=2000, gen_every=200, gen_tokens=200;
    GPTConfig cfg; gpt_config_default(&cfg);
    cfg.d_model=128;cfg.n_heads=4;cfg.n_layers=4;cfg.block_size=64;cfg.batch_size=32;cfg.lr=5e-4f;
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"--data")&&i+1<argc)data_path=argv[++i];
        else if(!strcmp(argv[i],"--out")&&i+1<argc)out_path=argv[++i];
        else if(!strcmp(argv[i],"--iters")&&i+1<argc)iters=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--lr")&&i+1<argc)cfg.lr=(float)atof(argv[++i]);
        else if(!strcmp(argv[i],"--d_model")&&i+1<argc)cfg.d_model=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--n_heads")&&i+1<argc)cfg.n_heads=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--n_layers")&&i+1<argc)cfg.n_layers=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--block_size")&&i+1<argc)cfg.block_size=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--batch_size")&&i+1<argc)cfg.batch_size=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--gen_every")&&i+1<argc)gen_every=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--gen_tokens")&&i+1<argc)gen_tokens=atoi(argv[++i]);
        else if(!strcmp(argv[i],"-h")||!strcmp(argv[i],"--help")){usage(argv[0]);return 0;}
    }
    cllm_rand_seed((uint64_t)time(NULL));
    printf("=== C-LLM: Tiny GPT in Pure C ===\n");
    CharDataset *ds = dataset_load(data_path);
    if(!ds){fprintf(stderr,"Load dataset failed\n");return 1;}
    cfg.vocab_size = ds->vocab_size;
    printf("Dataset: %lld chars, vocab=%d\n", ds->length, ds->vocab_size);
    printf("Vocab: ");
    for(int i=0;i<ds->vocab_size;i++){
        char c=ds->itos[i];
        if(c=='\n')printf("\\n"); else if(c==' ')printf("·"); else putchar(c);
    }printf("\n");
    GPTModel *m = gpt_create(&cfg);
    printf("Model: d=%d heads=%d layers=%d T=%d B=%d lr=%g params=%lld (%.2fM)\n",
           cfg.d_model,cfg.n_heads,cfg.n_layers,cfg.block_size,cfg.batch_size,cfg.lr,
           gpt_param_count(m),gpt_param_count(m)/1e6);

    int B=cfg.batch_size,T=cfg.block_size;
    int *inputs=(int*)malloc(B*T*sizeof(int));
    int *targets=(int*)malloc(B*T*sizeof(int));
    int *gen=(int*)malloc((size_t)(20+gen_tokens)*sizeof(int));
    printf("\nTraining %d iters...\n", iters);
    clock_t t0=clock();
    float running=0;
    for(int s=0;s<iters;s++){
        dataset_get_batch(ds,B,T,inputs,targets);
        float loss = gpt_forward_backward(m,inputs,targets);
        gpt_step(m);
        running = (s==0)?loss:running*0.99f+loss*0.01f;
        if(s%50==0){
            double el=(double)(clock()-t0)/CLOCKS_PER_SEC;
            printf("step %4d | loss %.4f (avg %.4f) | %.1fs\n",s,loss,running,el);
        }
        if(gen_every>0&&(s+1)%gen_every==0){
            int plen=10;
            for(int i=0;i<plen;i++)gen[i]=ds->tokens[i%100];
            gpt_generate(m,gen,plen,gen,gen_tokens,0.8f);
            printf("\n--- sample @ step %d ---\n",s+1);
            for(int i=0;i<plen+gen_tokens;i++)putchar(ds->itos[gen[i]]);
            printf("\n------------------------\n\n");
            fflush(stdout);
        }
    }
    gpt_save(m,out_path);
    printf("Model saved to %s\n", out_path);
    printf("\n=== Final 500-token sample ===\n");
    int plen=20;
    for(int i=0;i<plen;i++)gen[i]=ds->tokens[i%200];
    gpt_generate(m,gen,plen,gen,500,0.8f);
    for(int i=0;i<plen+500;i++)putchar(ds->itos[gen[i]]);
    printf("\n");
    free(inputs);free(targets);free(gen);
    dataset_free(ds);gpt_free(m);
    return 0;
}
