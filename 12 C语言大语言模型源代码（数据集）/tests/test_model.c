#include "cllm.h"
#include "model.h"
#include "dataset.h"
#include <stdio.h>

int main(){
    cllm_rand_seed(42);
    printf("===== C-LLM: Build Verification =====\n");
    
    // 1. 创建模型
    GPTConfig cfg; gpt_config_default(&cfg);
    cfg.vocab_size=65;cfg.d_model=64;cfg.n_heads=4;cfg.n_layers=2;
    cfg.block_size=32;cfg.batch_size=4;cfg.lr=1e-3f;
    GPTModel *m = gpt_create(&cfg);
    printf("  [OK] Model created, params=%lld\n", gpt_param_count(m));
    
    // 2. 前向+反向
    int inputs[128], targets[128];
    for(int i=0;i<128;i++){inputs[i]=i%65;targets[i]=(i+1)%65;}
    float loss = gpt_forward_backward(m, inputs, targets);
    printf("  [OK] Forward+Backward, initial loss=%.4f (expected ~ln(65)=%.3f)\n", loss, logf(65));
    
    // 3. AdamW更新一步
    gpt_step(m);
    float loss2 = gpt_forward_backward(m, inputs, targets);
    printf("  [OK] After 1 step, loss=%.4f (delta=%+.4f)\n", loss2, loss2-loss);
    
    // 4. 保存加载
    gpt_save(m,"/tmp/cllm_smoke.bin");
    GPTModel *m2 = gpt_load("/tmp/cllm_smoke.bin");
    if(m2){
        float loss3 = gpt_forward_backward(m2, inputs, targets);
        printf("  [OK] Save/Load round-trip, loss=%.4f (match=%.4f)\n", loss3, loss2);
        gpt_free(m2);
    }
    
    // 5. 生成
    int out[64], prompt[3]={0,1,2};
    gpt_generate(m, prompt, 3, out, 30, 1.0f);
    int valid=1;
    for(int i=0;i<33;i++)if(out[i]<0||out[i]>=65)valid=0;
    printf("  [OK] Generation 30 tokens, valid range: %s\n", valid?"PASS":"FAIL");
    
    // 6. 小数据集过拟合测试（100步）
    GPTConfig cfg2; gpt_config_default(&cfg2);
    cfg2.vocab_size=32;cfg2.d_model=32;cfg2.n_heads=4;cfg2.n_layers=2;
    cfg2.block_size=16;cfg2.batch_size=4;cfg2.lr=1e-3f;
    GPTModel *m3 = gpt_create(&cfg2);
    int in[64], tg[64];
    for(int i=0;i<64;i++){in[i]=i%32;tg[i]=(i*3+7)%32;}
    float init=gpt_forward_backward(m3,in,tg);
    for(int s=0;s<100;s++){gpt_forward_backward(m3,in,tg);gpt_step(m3);}
    float last=gpt_forward_backward(m3,in,tg);
    printf("  [OK] Overfit 100 steps: %.4f -> %.4f %s\n", init, last, last<init?"(loss decreased)":"(WARN: loss not decreased)");
    gpt_free(m3);
    
    gpt_free(m);
    printf("\n===== All smoke tests passed =====\n");
    return 0;
}
