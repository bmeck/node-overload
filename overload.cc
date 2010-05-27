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
	Handle<Object> holder=info.Holder();
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
	Handle<Value> data = holder->GetInternalField(1);
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
	Handle<Object> holder=info.Holder();
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

	//Set up arguments
	Handle<Value> values[4] = {property,old_value,value,had_value};

	//Grab function and call (property,value)
	Handle<Value> data = holder->GetInternalField(2);
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.Holder(),4,values);

	holder->ForceSet(property,new_value);

	//Return value is the return of the function call
	return scope.Close(new_value);
}

Handle<Value> WatchableIndexedPropertyGetter(
	uint32_t index
	, const AccessorInfo& info
	) {
	HandleScope scope;

	//Grab the value
	Handle<Object> holder=info.Holder();
	Handle<Number> property = Number::New(index);
	Handle<Value> value;
	Handle<Value> had_value;

	Handle<Object> indices = Handle<Object>::Cast(holder->GetInternalField(0));
	if( indices->Has(index) )  {
		value =indices->Get(index);
		had_value = True();
	}
	else {
		value = Undefined();
		had_value = False();
	}
	//Set up arguments
	Handle<Value> values[3] = {property,value,had_value};

	//Grab function and call (property,value)
	Handle<Value> data = holder->GetInternalField(1);
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.Holder(),3,values);

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
	Handle<Object> holder=info.Holder();
	Handle<Number> property = Number::New(index);
	Handle<Value> had_value;

	Handle<Object> indices = Handle<Object>::Cast(holder->GetInternalField(0));
	Handle<Value> old_value;
	if( indices->Has(index) )  {
		old_value = indices->Get(index);
		had_value = True();
	}
	else {
		old_value = Undefined();
		had_value = False();
	}

	//Set up arguments
	Handle<Value> values[4] = {property,old_value,value,had_value};

	//Grab function and call (property,value)
	Handle<Value> data = holder->GetInternalField(2);
	Handle<Function> callback = Handle<Function>::Cast(data);
	Handle<Value> new_value = callback->Call(info.Holder(),4,values);

	indices->Set(index,new_value);

	//Return value is the return of the function call
	return scope.Close(new_value);
}


Handle<Value> Watchable(const Arguments& args) {
	HandleScope scope;
	Handle<Value> getter = args[0];
	//Check our arguments!
	if(getter.IsEmpty() || getter->IsFunction()) {}
	else {
		return ThrowException(String::New("Getter callback must be a function"));
	}
	Handle<Value> setter = args[1];
	if(setter.IsEmpty() || setter->IsFunction()) {}
	else {
		return ThrowException(String::New("Getter callback must be a function"));
	}
	//Every one with a different callback needs a different template
	Local<ObjectTemplate> object_template = ObjectTemplate::New();
	//0 - Indices object
	//1 - Getter
	//2 - Setter
	//3 - Query
	//4 - Deleter
	//5 - Enumerator
	object_template->SetInternalFieldCount(5);
	object_template->SetNamedPropertyHandler(
		WatchableNamedPropertyGetter
		,WatchableNamedPropertySetter
//		,WatchableNamedPropertyQuery
//		,WatchableNamedPropertyDeleter
//		,WatchableNamedPropertyEnumerator
	);
	object_template->SetIndexedPropertyHandler(
		WatchableIndexedPropertyGetter
		,WatchableIndexedPropertySetter
		,0,0,0
	);
	Handle<Object> watchable = object_template->NewInstance();
	watchable->SetInternalField(0,Object::New());
	watchable->SetInternalField(1,getter);
	watchable->SetInternalField(2,setter);
	return scope.Close(watchable);
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	Local<FunctionTemplate> watchable_template = FunctionTemplate::New(Watchable);
	Local<Function> watchable = watchable_template->GetFunction();
	//Export
	target->Set(String::New("Watchable"),watchable);
}
