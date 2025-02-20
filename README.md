# mecab-gyp
nan対応してるのでnodeの仕様変更にも強いはず

- 1.0.6~ nodejs12以降に対応
- 1.1.0~ nodejs18 python3に対応
- 1.2.0~ nodejs22 python3.12以降に対応

# How to install

```
npm install mecab-gyp
```

# How to use

```
const MecabGyp = new require('mecab-gyp');
var mecabGyp = new MecabGyp('');
var results = mecabGyp.parseAsNode('本日は晴天なり');
```

results
```
[ { id: 0,
    surface: '',
    feature: 'BOS/EOS,*,*,*,*,*,*,*,*',
    len: 0,
    rcAttr: 0,
    lcAttr: 0,
    posid: 0,
    char_type: 0,
    stat: 2,
    isbest: 1,
    alpha: 0,
    beta: 0,
    prob: 0,
    cost: 0 },
  { id: 10,
    surface: '本日は晴天なり',
    feature: '名詞,固有名詞,人名,一般,*,*,本日は晴天なり,ホンジツハセイテンナリ,ホンジツハセイテンナリ',
    len: 21,
    rcAttr: 1289,
    lcAttr: 1289,
    posid: 42,
    char_type: 2,
    stat: 0,
    isbest: 1,
    alpha: 0,
    beta: 0,
    prob: 0,
    cost: 1159 },
    :
    :
```

# MecabGypオプション
Mecab::createTaggerにそのまま流れます。

例えばこの様に使います
```
var mecabGyp = new MecabGyp('--dicdir=/PathTo/mecabdic/');

```
