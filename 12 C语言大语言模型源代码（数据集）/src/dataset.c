#include "dataset.h"

// 内置tiny-shakespeare样本（1126字符，保证没有数据集文件也能直接训练）
static const char *TINY_SHAKESPEARE =
"ROMEO:\nBut, soft! what light through yonder window breaks?\n"
"It is the east, and Juliet is the sun.\n"
"Arise, fair sun, and kill the envious moon,\n"
"Who is already sick and pale with grief,\n"
"That thou her maid art far more fair than she:\n"
"Be not her maid, since she is envious;\n"
"Her vestal livery is but sick and green\n"
"And none but fools do wear it; cast it off.\n"
"It is my lady, O, it is my love!\n"
"O, that she knew she were!\n"
"She speaks yet she says nothing: what of that?\n"
"Her eye discourses; I will answer it.\n"
"I am too bold, 'tis not to me she speaks:\n"
"Two of the fairest stars in all the heaven,\n"
"Having some business, do entreat her eyes\n"
"To twinkle in their spheres till they return.\n"
"What if her eyes were there, they in her head?\n"
"The brightness of her cheek would shame those stars,\n"
"As daylight doth a lamp; her eyes in heaven\n"
"Would through the airy region stream so bright\n"
"That birds would sing and think it were not night.\n"
"See, how she leans her cheek upon her hand!\n"
"O, that I were a glove upon that hand,\n"
"That I might touch that cheek!\n\n"
"JULIET:\n"
"O Romeo, Romeo! wherefore art thou Romeo?\n"
"Deny thy father and refuse thy name;\n"
"Or, if thou wilt not, be but sworn my love,\n"
"And I'll no longer be a Capulet.\n\n"
"ROMEO:\n"
"[Aside] Shall I hear more, or shall I speak at this?\n\n"
"JULIET:\n"
"'Tis but thy name that is my enemy;\n"
"Thou art thyself, though not a Montague.\n"
"What's Montague? it is nor hand, nor foot,\n"
"Nor arm, nor face, nor any other part\n"
"Belonging to a man. O, be some other name!\n"
"What's in a name? that which we call a rose\n"
"By any other name would smell as sweet;\n"
"So Romeo would, were he not Romeo call'd,\n"
"Retain that dear perfection which he owes\n"
"Without that title. Romeo, doff thy name,\n"
"And for that name which is no part of thee\n"
"Take all myself.\n\n"
"To be, or not to be: that is the question:\n"
"Whether 'tis nobler in the mind to suffer\n"
"The slings and arrows of outrageous fortune,\n"
"Or to take arms against a sea of troubles,\n"
"And by opposing end them? To die: to sleep;\n"
"No more; and by a sleep to say we end\n"
"The heart-ache and the thousand natural shocks\n"
"That flesh is heir to, 'tis a consummation\n"
"Devoutly to be wish'd. To die, to sleep;\n"
"To sleep: perchance to dream: ay, there's the rub;\n"
"For in that sleep of death what dreams may come\n"
"When we have shuffled off this mortal coil,\n"
"Must give us pause.\n\n"
"All the world's a stage,\n"
"And all the men and women merely players:\n"
"They have their exits and their entrances;\n"
"And one man in his time plays many parts,\n"
"His acts being seven ages.\n\n"
"Friends, Romans, countrymen, lend me your ears;\n"
"I come to bury Caesar, not to praise him.\n"
"The evil that men do lives after them;\n"
"The good is oft interred with their bones;\n"
"So let it be with Caesar.\n\n"
"Now is the winter of our discontent\n"
"Made glorious summer by this sun of York;\n"
"And all the clouds that lour'd upon our house\n"
"In the deep bosom of the ocean buried.\n";

void dataset_write_tiny_shakespeare(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp) { fclose(fp); return; } // 已存在
    fp = fopen(path, "wb");
    if (!fp) return;
    fwrite(TINY_SHAKESPEARE, 1, strlen(TINY_SHAKESPEARE), fp);
    fclose(fp);
}

CharDataset* dataset_load(const char *path) {
    dataset_write_tiny_shakespeare(path);
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    CharDataset *ds = (CharDataset*)calloc(1, sizeof(CharDataset));
    ds->text = (char*)malloc(sz+1);
    ds->length = (long long)fread(ds->text, 1, sz, fp);
    ds->text[ds->length] = '\0';
    fclose(fp);

    // 构建词表
    int used[256] = {0};
    for (long long i = 0; i < ds->length; i++) used[(unsigned char)ds->text[i]] = 1;
    int idx = 0;
    for (int i = 0; i < 256; i++) {
        ds->stoi[i] = -1;
        if (used[i]) {
            ds->stoi[i] = idx;
            ds->itos[idx] = (char)i;
            idx++;
        }
    }
    ds->vocab_size = idx;

    // tokenize
    ds->tokens = (int*)malloc(sizeof(int) * ds->length);
    for (long long i = 0; i < ds->length; i++) {
        ds->tokens[i] = ds->stoi[(unsigned char)ds->text[i]];
    }
    return ds;
}

void dataset_free(CharDataset *ds) {
    if (!ds) return;
    free(ds->text); free(ds->tokens); free(ds);
}

void dataset_get_batch(CharDataset *ds, int B, int T, int *inputs, int *targets) {
    long long max_start = ds->length - T - 1;
    if (max_start < 1) max_start = 1;
    for (int b = 0; b < B; b++) {
        long long start = (long long)(cllm_rand_uniform(0.0f, 1.0f) * (max_start-1));
        for (int t = 0; t < T; t++) {
            inputs[b*T + t] = ds->tokens[start + t];
            targets[b*T + t] = ds->tokens[start + t + 1];
        }
    }
}
