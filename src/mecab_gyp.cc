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
  ~MecabGyp() {}

  MeCab::Tagger* getTagger() {
    if(this->tagger) {
      return this->tagger;
    }
    this->tagger = MeCab::createTagger(this->options.c_str());
    return this->tagger;
  }

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      v8::String::Utf8Value options(info[0]);
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
    v8::String::Utf8Value input(info[0]);
    const MeCab::Node* node = mecabGyp->getTagger()->parseToNode(*input);
    std::vector<v8::Local<v8::Object>> nodes;
    while(node) {
      v8::Local<v8::Object> obj = Nan::New<v8::Object>();
      obj->Set(Nan::New("id").ToLocalChecked(), Nan::New(node->id));
      obj->Set(Nan::New("surface").ToLocalChecked(), Nan::New(std::string(node->surface, node->length)).ToLocalChecked());
      obj->Set(Nan::New("feature").ToLocalChecked(), Nan::New(node->feature).ToLocalChecked());
      obj->Set(Nan::New("len").ToLocalChecked(), Nan::New(node->length));
      obj->Set(Nan::New("rcAttr").ToLocalChecked(), Nan::New(node->rcAttr));
      obj->Set(Nan::New("lcAttr").ToLocalChecked(), Nan::New(node->lcAttr));
      obj->Set(Nan::New("posid").ToLocalChecked(), Nan::New(node->posid));
      obj->Set(Nan::New("char_type").ToLocalChecked(), Nan::New(node->char_type));
      obj->Set(Nan::New("stat").ToLocalChecked(), Nan::New(node->stat));
      obj->Set(Nan::New("isbest").ToLocalChecked(), Nan::New(node->isbest));
      obj->Set(Nan::New("alpha").ToLocalChecked(), Nan::New(node->alpha));
      obj->Set(Nan::New("beta").ToLocalChecked(), Nan::New(node->beta));
      obj->Set(Nan::New("prob").ToLocalChecked(), Nan::New(node->prob));
      obj->Set(Nan::New("cost").ToLocalChecked(), Nan::New((int)node->cost));
      nodes.push_back(obj);
      node = node->next;
    }
    v8::Local<v8::Array> results = Nan::New<v8::Array>(nodes.size());
    for(unsigned long i = 0; i < nodes.size(); i++) {
      results->Set(i, nodes.at(i));
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
