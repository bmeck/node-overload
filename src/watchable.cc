//
//	node-watchable
//	@exports
//		Watchabe(onGet) - Supplies getter and setter callbacks across all the properties
//
//	@example
//		//print out any property gets that use the . operator
//		var debug = Watchable(function(p,v){if(typeof v == "number") return v; sys.puts(p);return v;})

#include <v8.h>
#include <map>
#include <iostream>

using namespace std;
using namespace v8;

Handle<Object> WatchableArgs(Handle<Value> holder, Handle<Value> thisObject) {
	HandleScope scope;
	Handle<Object> result = Object::New();
	result->Set(String::New("holder"),holder);
	result->Set(String::New("thisObject"),thisObject);
	return scope.Close(result);
}

Handle<Value> WatchableNamedPropertyGetter(
	Local<String> property
	, const AccessorInfo& info
	) {
		HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(0);
	if(data->IsNull()) {
		return scope.Close(Undefined());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),property);
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Value> WatchableNamedPropertySetter(
	Local<String> property
	, Local<Value> value
	, const AccessorInfo& info
	) {	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(1);
	if(data->IsNull()) {
		return scope.Close(Undefined());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),property);
	args->Set(String::New("value"),value);
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Array> WatchableNamedPropertyEnumerator(
	const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(4);
	if(data->IsNull()) {
		return scope.Close(Array::New());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	printf("test\n");

	if(new_value->IsArray()) {
		printf("ok?\n");
		return scope.Close(Handle<Array>::Cast(new_value));
	}
	else {
		printf("wtf\n");
		ThrowException(Exception::Error(String::New("Callback must return an Array.")));
		return scope.Close(Array::New());
	}
}
Handle<Integer> WatchableNamedPropertyQuery(
	Local<String> property
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(2);
	if(data->IsNull()) {
		return scope.Close(Integer::New(0));
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),property);
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call

	//Return value is the return of the function call
	return scope.Close(Integer::New(
		new_value->IsTrue() ? v8::None : 0
	));
}

Handle<Boolean> WatchableNamedPropertyDeleter(
	Local<String> property
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(3);
	if(data->IsNull()) {
		return scope.Close(False());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),property);
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Value> WatchableIndexedPropertyGetter(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(0);
	if(data->IsNull()) {
		return scope.Close(False());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),Integer::New(index));
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(new_value);
}


Handle<Value> WatchableIndexedPropertySetter(
	uint32_t index
	, Local<Value> value
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(1);
	if(data->IsNull()) {
		printf("defaulted setter");
		return scope.Close(Undefined());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),Integer::New(index));
	args->Set(String::New("value"),value);
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(new_value);
}
Handle<Array> WatchableIndexedPropertyEnumerator(
	const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(4);
	if(data->IsNull()) {
		return scope.Close(Array::New());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	if(new_value->IsArray()) {
		return scope.Close(Handle<Array>::Cast(new_value));
	}
	else {
		ThrowException(String::New("Callback must return an Array."));
		return scope.Close(Array::New());
	}
}
Handle<Boolean> WatchableIndexedPropertyQuery(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(2);
	if(data->IsNull()) {
		return scope.Close(False());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),Integer::New(index));
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Boolean> WatchableIndexedPropertyDeleter(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(info.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(3);
	if(data->IsNull()) {
		return scope.Close(False());
	}
	//Set up arguments
	Handle<Object> args=WatchableArgs(info.Holder(),info.This());
	args->Set(String::New("property"),Integer::New(index));
	Handle<Value> values[1] = {args};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.This(),1,values);
	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Value> WatchableCallback(const Arguments& args) {
	HandleScope scope;

	//Grab the value
	Handle<Object> callbacks = Handle<Object>::Cast(args.Data());
	//Default
	Handle<Value> data = callbacks->GetInternalField(args.IsConstructCall()?5:6);
	if(data->IsNull()) {
		return scope.Close(Undefined());
	}
	//Set up arguments
	Handle<Object> obj = WatchableArgs(args.Holder(),args.This());
	obj->Set(String::New("construction"),Boolean::New(args.IsConstructCall()));
	Handle<Value> values[1] = {obj};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(args.This(),1,values);

	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Value> Watchable(const Arguments& args) {
	HandleScope scope;
	//Check our arguments!
	Handle<Value> getter = args[0];
	if(getter->IsFunction()) {
		//Do nothing
	}
	else if(getter->IsNull() || getter->IsUndefined()) {
		getter = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Getter callback must be a function"))));
	}
	Handle<Value> setter = args[1];
	if(setter->IsFunction()) {
		//Do nothing
	}
	else if(setter->IsNull() || setter->IsUndefined()) {
		setter = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Setter callback must be a function"))));
	}
	Handle<Value> enumerator = args[2];
	if(enumerator->IsFunction()) {
		//Do nothing
	}
	else if(enumerator->IsNull() || enumerator->IsUndefined()) {
		enumerator = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Enumerator callback must be a function"))));
	}
	Handle<Value> query = args[3];
	if(query->IsFunction()) {
		//Do nothing
	}
	else if(query->IsNull() || query->IsUndefined()) {
		query = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Query callback must be a function"))));
	}
	Handle<Value> deleter = args[4];
	if(deleter->IsFunction()) {
		//Do nothing
	}
	else if(deleter->IsNull() || deleter->IsUndefined()) {
		deleter = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Deleter callback must be a function"))));
	}
	Handle<Value> caller = args[5];
	if(caller->IsFunction()) {
		//Do nothing
	}
	else if(caller->IsNull() || caller->IsUndefined()) {
		caller = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Caller callback must be a function"))));
	}
	Handle<Value> constructor = args[6];
	if(constructor->IsFunction()) {
		//Do nothing
	}
	else if(constructor->IsNull() || constructor->IsUndefined()) {
		constructor = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Constructor callback must be a function"))));
	}
	Local<ObjectTemplate> callbackHolder_template = ObjectTemplate::New();
	callbackHolder_template->SetInternalFieldCount(7);
	Handle<Object> callbackHolder = callbackHolder_template->NewInstance();
	//Every one with a different callback needs a different template
	//0 - Holder object - aka the real one
	//1 - Getter
	//2 - Setter
	//3 - Query
	//4 - Deleter
	//5 - Enumerator
	callbackHolder->SetInternalField(0,getter);
	callbackHolder->SetInternalField(1,setter);
	callbackHolder->SetInternalField(2,query);
	callbackHolder->SetInternalField(3,deleter);
	callbackHolder->SetInternalField(4,enumerator);
	callbackHolder->SetInternalField(5,caller);
	callbackHolder->SetInternalField(6,constructor);
	Local<ObjectTemplate> object_template = ObjectTemplate::New();
	object_template->SetNamedPropertyHandler(
		WatchableNamedPropertyGetter
		,WatchableNamedPropertySetter
		,WatchableNamedPropertyQuery
		,WatchableNamedPropertyDeleter
		,WatchableNamedPropertyEnumerator
		,callbackHolder
	);
	object_template->SetIndexedPropertyHandler(
		WatchableIndexedPropertyGetter
		,WatchableIndexedPropertySetter
		,WatchableIndexedPropertyQuery
		,WatchableIndexedPropertyDeleter
		,0//WatchableIndexedPropertyEnumerator
		,callbackHolder
	);
	object_template->SetCallAsFunctionHandler(
		WatchableCallback
		,callbackHolder
	);
	Handle<Object> watchable = object_template->NewInstance();
	////printf("watchable created\n");
	return scope.Close(watchable);
}
Handle<Value> RePrototype(const Arguments& args) {
	HandleScope scope;
	if(args[0]->IsObject()&&args[1]->IsObject()) {
		Handle<Object> obj=Handle<Object>::Cast(args[0]);
		return scope.Close(Boolean::New(obj->SetPrototype(args[1])));
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Both arguments must be Objects"))));
	}
}

//Handle<Function> Context(const Arguments& args) {
//	HandleScope scope;
//	if(args[0]->IsFunction()) {
//		Handle<Context> restricted = Conetext::New();
//		Handle<Object> obj=Handle<Object>::Cast(args[0]);
//		return scope.Close(args[0]);
//	}
//	else {
//		return scope.Close(ThrowException(Exception::Error(String::New("Argument must be a function"))));
//	}
//}