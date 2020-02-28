#include <string>
#include <iostream>
#include <mecab.h>
#include <nan.h>

class MecabGyp : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("MecabGyp").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "parseAsNode", ParseAsNode);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("MecabGyp").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
  explicit MecabGyp(const char *value) : tagger(0) {
    this->options = value;
  }
  ~MecabGyp() {
    delete this->tagger;
    this->tagger = 0;
  }

  MeCab::Tagger* getTagger() {
    if(this->tagger) {
      return this->tagger;
    }
    this->tagger = MeCab::createTagger(this->options.c_str());
    return this->tagger;
  }

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      Nan::Utf8String options(Nan::To<v8::String>((info[0])).ToLocalChecked());
      MecabGyp *obj = new MecabGyp(*options);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(ParseAsNode) {
    MecabGyp* mecabGyp = Nan::ObjectWrap::Unwrap<MecabGyp>(info.Holder());
    Nan::Utf8String input(Nan::To<v8::String>((info[0])).ToLocalChecked());
    const MeCab::Node* node = mecabGyp->getTagger()->parseToNode(*input);
    std::vector<v8::Local<v8::Object>> nodes;
    while(node) {
      v8::Local<v8::Object> obj = Nan::New<v8::Object>();
      Nan::Set(obj, Nan::New("id").ToLocalChecked(), Nan::New(node->id));
      Nan::Set(obj, Nan::New("surface").ToLocalChecked(), Nan::New(std::string(node->surface, node->length)).ToLocalChecked());
      Nan::Set(obj, Nan::New("feature").ToLocalChecked(), Nan::New(node->feature).ToLocalChecked());
      Nan::Set(obj, Nan::New("len").ToLocalChecked(), Nan::New(node->length));
      Nan::Set(obj, Nan::New("rcAttr").ToLocalChecked(), Nan::New(node->rcAttr));
      Nan::Set(obj, Nan::New("lcAttr").ToLocalChecked(), Nan::New(node->lcAttr));
      Nan::Set(obj, Nan::New("posid").ToLocalChecked(), Nan::New(node->posid));
      Nan::Set(obj, Nan::New("char_type").ToLocalChecked(), Nan::New(node->char_type));
      Nan::Set(obj, Nan::New("stat").ToLocalChecked(), Nan::New(node->stat));
      Nan::Set(obj, Nan::New("isbest").ToLocalChecked(), Nan::New(node->isbest));
      Nan::Set(obj, Nan::New("alpha").ToLocalChecked(), Nan::New(node->alpha));
      Nan::Set(obj, Nan::New("beta").ToLocalChecked(), Nan::New(node->beta));
      Nan::Set(obj, Nan::New("prob").ToLocalChecked(), Nan::New(node->prob));
      Nan::Set(obj, Nan::New("cost").ToLocalChecked(), Nan::New((int)node->cost));
      nodes.push_back(obj);
      node = node->next;
    }
    v8::Local<v8::Array> results = Nan::New<v8::Array>(nodes.size());
    for(unsigned long i = 0; i < nodes.size(); i++) {
      Nan::Set(results, i, nodes.at(i));
    }
    info.GetReturnValue().Set(results);
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> c;
    return c;
  }

  MeCab::Tagger *tagger;
  std::string options;
};

NODE_MODULE(mecab_gyp, MecabGyp::Init)
