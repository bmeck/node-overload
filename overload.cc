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

Handle<Value> WatchableNamedPropertyGetter(
	Local<String> property
	, const AccessorInfo& info
	) {
	HandleScope scope;
	//Grab the value of the property
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(1);
	if(data->IsNull()) {
		return scope.Close(map->Get(property));
	}
	Handle<Value> value;
	Handle<Value> had_value;

	if(holder->HasRealNamedProperty(property)) {
		value = holder->GetRealNamedProperty(property);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}

	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.Holder(),3,values);

	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Value> WatchableNamedPropertySetter(
	Local<String> property
	, Local<Value> value
	, const AccessorInfo& info
	) {
	HandleScope scope;
	//Grab the value of the property
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	Handle<Value> old_value;
	Handle<Value> had_value;

	if(holder->HasRealNamedProperty(property)) {
		old_value = holder->GetRealNamedProperty(property);
		had_value = True();
	}
	else {
		old_value = Undefined();
		had_value = False();
	}
	//Default
	Handle<Value> data = holder->GetInternalField(2);
	if(data->IsNull()) {
		return scope.Close(map->Set(property,value)
			?value
			:old_value
		);
	}

	//Set up arguments
	Handle<Value> values[4] = {property,old_value,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,4,values);

	holder->ForceSet(property,new_value);

	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Array> WatchableNamedPropertyEnumerator(
	const AccessorInfo& info
	) {
	HandleScope scope;
		//printf("named enum\n");
	//Grab the value of the property
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(5);
	if(data->IsNull()) {
		//printf("defaulting indexed enum\n");
		return scope.Close(map->GetPropertyNames());
	}
	Handle<Value> old_value;
	Handle<Value> had_value;

	//Set up arguments
	Handle<Value> values[0] = {};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,0,values);

	if(new_value->IsArray()) {
		return scope.Close(Handle<Array>::Cast(new_value));
	}
	else {
		ThrowException(Exception::Error(String::New("Callback must return an Array.")));
		return scope.Close(Array::New());
	}
}
Handle<Boolean> WatchableNamedPropertyQuery(
	Local<String> property
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(3);
	if(data->IsNull()) {
		return scope.Close(Boolean::New(holder->Has(property)));
	}
	Handle<Value> value;
	Handle<Value> had_value;

	if( holder->Has(property) )  {
		value = map->Get(property);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,3,values);

	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Boolean> WatchableNamedPropertyDeleter(
	Local<String> property
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(4);
	if(data->IsNull()) {
		return scope.Close(Boolean::New(holder->Delete(property)));
	}
	Handle<Value> value;
	Handle<Value> had_value;

	if( holder->Has(property) )  {
		value = map->Get(property);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,3,values);

	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Value> WatchableIndexedPropertyGetter(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(1);
	if(data->IsNull()) {
		return scope.Close(map->Get(index));
	}
	Handle<Number> property = Number::New(index);
	Handle<Value> value;
	Handle<Value> had_value;

	if( holder->Has(index) )  {
		value = map->Get(index);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,3,values);

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
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	Handle<Number> property = Number::New(index);
	Handle<Value> had_value;

	Handle<Value> old_value;
	if( holder->Has(index) )  {
		old_value = map->Get(index);
		had_value = True();
	}
	else {
		old_value = Undefined();
		had_value = False();
	}
	//Default
	Handle<Value> data = holder->GetInternalField(2);
	if(data->IsNull()) {
		return scope.Close(map->Set(index,value)
			?value
			:old_value
		);
	}
	//Set up arguments
	Handle<Value> values[4] = {property,old_value,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,4,values);

	map->Set(index,new_value);

	//Return value is the return of the function call
	return scope.Close(new_value);
}
Handle<Array> WatchableIndexedPropertyEnumerator(
	const AccessorInfo& info
	) {
	HandleScope scope;
	//Grab the value
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(5);
	if(data->IsNull()) {
		return scope.Close(map->GetPropertyNames());
	}
	//Set up arguments
	Handle<Value> values[0];

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,0,values);

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
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(3);
	if(data->IsNull()) {
		return scope.Close(Boolean::New(holder->Has(index)));
	}
	Handle<Number> property = Integer::New(index);
	Handle<Value> value;
	Handle<Value> had_value;

	if( holder->Has(index) )  {
		value = map->Get(index);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,3,values);

	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
}

Handle<Boolean> WatchableIndexedPropertyDeleter(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> holder = info.Holder();
	Handle<Object> map = Handle<Object>::Cast(
		holder->GetInternalField(0)
	);
	//Default
	Handle<Value> data = holder->GetInternalField(4);
	if(data->IsNull()) {
		return scope.Close(Boolean::New(holder->Delete(index)));
	}
	Handle<Number> property = Integer::New(index);
	Handle<Value> value;
	Handle<Value> had_value;

	if( holder->Has(index) )  {
		value = map->Get(index);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(holder,3,values);

	//Return value is the return of the function call
	return scope.Close(Boolean::New(new_value->IsTrue()));
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
	else if(query->IsNull() || enumerator->IsUndefined()) {
		query = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Query callback must be a function"))));
	}
	Handle<Value> deleter = args[4];
	if(deleter->IsFunction()) {
		//Do nothing
	}
	else if(deleter->IsNull() || enumerator->IsUndefined()) {
		deleter = Null();
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Deleter callback must be a function"))));
	}
	
	Local<ObjectTemplate> object_template = ObjectTemplate::New();
	object_template->SetNamedPropertyHandler(
		WatchableNamedPropertyGetter
		,WatchableNamedPropertySetter
		,WatchableNamedPropertyQuery
		,WatchableNamedPropertyDeleter
		,WatchableNamedPropertyEnumerator
	);
	object_template->SetIndexedPropertyHandler(
		WatchableIndexedPropertyGetter
		,WatchableIndexedPropertySetter
		,WatchableIndexedPropertyQuery
		,WatchableIndexedPropertyDeleter
		,WatchableIndexedPropertyEnumerator
	);
	//Every one with a different callback needs a different template
	//0 - Holder object - aka the real one
	//1 - Getter
	//2 - Setter
	//3 - Query
	//4 - Deleter
	//5 - Enumerator
	object_template->SetInternalFieldCount(6);
	Handle<Object> watchable = object_template->NewInstance();
	watchable->SetInternalField(0,Object::New());
	watchable->SetInternalField(1,getter);
	watchable->SetInternalField(2,setter);
	watchable->SetInternalField(3,query);
	watchable->SetInternalField(4,deleter);
	watchable->SetInternalField(5,enumerator);
	////printf("watchable created\n");
	return scope.Close(watchable);
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	////printf("init\n");
	Local<FunctionTemplate> watchable_template = FunctionTemplate::New(Watchable);
	Local<Function> watchable = watchable_template->GetFunction();
	//Export
	target->Set(String::New("Watchable"),watchable);
}
