#include <nan.h>
#include "map.h"

namespace SassTypes
{
  Map::Map(Sass_Value* v) : SassValueWrapper(v) {}

  Sass_Value* Map::construct(const std::vector<v8::Local<v8::Value>> raw_val) {
    size_t length = 0;

    if (raw_val.size() >= 1) {
      if (!raw_val[0]->IsNumber()) {
        throw std::invalid_argument("First argument should be an integer.");
      }

      length = raw_val[0]->ToInt32()->Value();
    }

    return sass_make_map(length);
  }

  void Map::initPrototype(v8::Local<v8::FunctionTemplate> proto) {
    Nan::SetPrototypeMethod(proto, "getLength", GetLength);
    Nan::SetPrototypeMethod(proto, "getKey", GetKey);
    Nan::SetPrototypeMethod(proto, "setKey", SetKey);
    Nan::SetPrototypeMethod(proto, "getValue", GetValue);
    Nan::SetPrototypeMethod(proto, "setValue", SetValue);
  }

  NAN_METHOD(Map::GetValue) {

    if (info.Length() != 1) {
      return Nan::ThrowError(Nan::New("Expected just one argument").ToLocalChecked());
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowError(Nan::New("Supplied index should be an integer").ToLocalChecked());
    }

    Sass_Value* map = unwrap(info.This())->value;
    size_t index = info[0]->ToInt32()->Value();


    if (index >= sass_map_get_length(map)) {
      return Nan::ThrowError(Nan::New("Out of bound index").ToLocalChecked());
    }

    info.GetReturnValue().Set(Factory::create(sass_map_get_value(map, info[0]->ToInt32()->Value()))->get_js_object());
  }

  NAN_METHOD(Map::SetValue) {
    if (info.Length() != 2) {
      return Nan::ThrowError(Nan::New("Expected two arguments").ToLocalChecked());
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowError(Nan::New("Supplied index should be an integer").ToLocalChecked());
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowError(Nan::New("Supplied value should be a SassValue object").ToLocalChecked());
    }

    Value* sass_value = Factory::unwrap(info[1]);
    sass_map_set_value(unwrap(info.This())->value, info[0]->ToInt32()->Value(), sass_value->get_sass_value());
    return;
  }

  NAN_METHOD(Map::GetKey) {

    if (info.Length() != 1) {
      return Nan::ThrowError(Nan::New("Expected just one argument").ToLocalChecked());
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowError(Nan::New("Supplied index should be an integer").ToLocalChecked());
    }

    Sass_Value* map = unwrap(info.This())->value;
    size_t index = info[0]->ToInt32()->Value();


    if (index >= sass_map_get_length(map)) {
      return Nan::ThrowError(Nan::New("Out of bound index").ToLocalChecked());
    }

    info.GetReturnValue().Set(Factory::create(sass_map_get_key(map, info[0]->ToInt32()->Value()))->get_js_object());
  }

  NAN_METHOD(Map::SetKey) {
    if (info.Length() != 2) {
      return Nan::ThrowError(Nan::New("Expected two arguments").ToLocalChecked());
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowError(Nan::New("Supplied index should be an integer").ToLocalChecked());
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowError(Nan::New("Supplied value should be a SassValue object").ToLocalChecked());
    }

    Value* sass_value = Factory::unwrap(info[1]);
    sass_map_set_key(unwrap(info.This())->value, info[0]->ToInt32()->Value(), sass_value->get_sass_value());
    return;
  }

  NAN_METHOD(Map::GetLength) {
    info.GetReturnValue().Set(Nan::New<v8::Number>(sass_map_get_length(unwrap(info.This())->value)));
  }
}
