//  http://wiki.ecmascript.org/doku.php?id=harmony:ephemeron_tables
//	node-overload
//	@exports
//		OnCollect(Object,function(theObject))
//

#include <v8.h>
#include <map>
#include <vector>
using namespace v8;
using namespace std;

typedef pair<Persistent<Object>*, Persistent<Value>*> EphemeronPair;
typedef vector<EphemeronPair > EphemeronVector;
typedef map<int,EphemeronVector*> EphemeronMap;

Persistent<ObjectTemplate> table_instance_template;

void OurWeakReferenceCallback(
		Persistent<Value> object
		, void* parameter
	) {
	HandleScope scope;
	Persistent<Function>* callback = (Persistent<Function>*)parameter;

	Handle<Value> values[0];
	values[0] = object;
	//object.ClearWeak();
	//puts("CB INVOKING");
	(*callback)->Call(Context::GetCurrent()->Global(),1,values);
	//puts("OBJ BRANCHING");
	if(object.IsNearDeath()) {
		object.Dispose();
		delete callback;
	}
	//callback->ClearWeak();
	//object.Dispose();
	//delete callback;
	scope.Close(Undefined());
}

void KeepingWeakReferenceCallback(
		Persistent<Value> callback
		, void* parameter
	) {
	HandleScope scope;
	Persistent<Object>* object = (Persistent<Object>*)parameter;
	//puts("CB BRANCHING");
	if(object->IsNearDeath()) {
		//puts("CB CLEARED");
		callback.Dispose();
		delete object;
	}
	else {
		//puts("CB REVIVED");
		//Revive this object so long as there is a dependant weak reference
		callback.MakeWeak(parameter,KeepingWeakReferenceCallback);
	}
	scope.Close(Undefined());
}

Handle<Value> Weak(
		const Arguments& args
	) {
	HandleScope scope;
	Handle<Value> obj = args[0];
	if(!obj->IsObject() || obj->IsNull() || obj->IsUndefined()) {
		return scope.Close(ThrowException(Exception::Error(String::New("First argument must be an object"))));
	}
	Handle<Value> onCollect = args[1];
	if(onCollect->IsFunction()) {
		Persistent<Object>* persistent_obj = new Persistent<Object>();
		*persistent_obj = Persistent<Object>::New(Handle<Object>::Cast(obj));
		Persistent<Function>* persistent_cb = new Persistent<Function>();
		*persistent_cb = Persistent<Function>::New(Handle<Function>::Cast(onCollect));

		persistent_obj->MakeWeak(
			persistent_cb
			, OurWeakReferenceCallback
		);
		//bind a weak ref of cb to the obj
		persistent_cb->MakeWeak(
			persistent_obj
			, KeepingWeakReferenceCallback
		);
		return scope.Close(*persistent_obj);
	}
	else {
		return scope.Close(ThrowException(Exception::Error(String::New("Collect callback must be a function"))));
	}
}

Handle<Object> WrapMap(EphemeronMap* obj,Handle<Object> table) {
	HandleScope scope;
	table->SetInternalField(
		0,
		External::New(obj)
	);
	return scope.Close(table);
}

EphemeronMap* UnwrapMap(Handle<Object> obj) {
	Handle<External> mymap_wrap = Handle<External>::Cast(obj->GetInternalField(0));
	void* mymap_ptr = mymap_wrap->Value();
	return static_cast<EphemeronMap*>(
		mymap_ptr
	);
}

//Remove the object's pair from the map
void EphemeronScrub(
		Persistent<Value> object
		, void* parameter
	) {
	EphemeronMap* mymap = static_cast<EphemeronMap*>(parameter);
	Handle<Object> obj = Handle<Object>::Cast(object);
	int id = obj->GetIdentityHash();
	EphemeronMap::iterator i = mymap->find(id);
	if(i != mymap->end()) {
		EphemeronVector* vector = (*i).second;
		EphemeronVector::iterator ii = vector->begin();
		while(ii != vector->end()) {
			if(obj->StrictEquals(*((*ii).first))) {
				vector->erase(ii);
				if(vector->size()==0) {
					mymap->erase(id);
				}
				object.Dispose();
				//puts("dispose + erase");
				return;
			}
			ii++;
		}
	}
	//puts("dispose");
	object.Dispose();
}

Handle<Value> EphemeronInit(
		const Arguments& args
	) {
	HandleScope scope;
	if(args.IsConstructCall()) {
		EphemeronMap* mymap = new EphemeronMap();
		return scope.Close(WrapMap(mymap,args.This()));
	}
	return scope.Close(ThrowException(Exception::Error(String::New("Must be called through the 'new' operator."))));
}

Handle<Value> EphemeronSet(
		const Arguments& args
	) {
	HandleScope scope;
	Handle<Value> key = args[0];
	if (key->IsUndefined() || key->IsNull()) {
		return scope.Close(ThrowException(Exception::Error(String::New("Key must be an Object"))));
	}
	Handle<Object> obj = key->ToObject();
	Handle<Value> value = args[1];

	EphemeronMap* mymap = UnwrapMap(args.This());

	Persistent<Object>* obj_ptr = new Persistent<Object>();
	*obj_ptr = Persistent<Object>::New(obj);
	obj_ptr->MakeWeak(mymap,EphemeronScrub);

	Persistent<Value>* value_ptr = new Persistent<Value>();
	*value_ptr = Persistent<Value>::New(value);
	value_ptr->MakeWeak(obj_ptr,KeepingWeakReferenceCallback);

	int id = obj->GetIdentityHash();

	EphemeronPair mypair = EphemeronPair(obj_ptr,value_ptr);
	EphemeronVector* vector = NULL;
	EphemeronMap::iterator i = mymap->find(id);

	if(i != mymap->end()) {
		vector = (*i).second;
		EphemeronVector::iterator ii = vector->begin();
		int j = 0;
		while(ii != vector->end()) {
			EphemeronPair original_pair = *ii;
			if((*original_pair.first)->StrictEquals(obj)) {
				vector->at(j) = mypair;
				return scope.Close(obj);
			}
			ii++;
			j++;
		}
		i++;
	}
	else {
		vector = new EphemeronVector();
		mymap->insert( pair<int,EphemeronVector*>(id,vector) );
	}
	vector->push_back( mypair );
	return scope.Close(obj);
}

Handle<Value> EphemeronGet(
		const Arguments& args
	) {
	HandleScope scope;
	Handle<Value> key = args[0];
	if (key->IsUndefined() || key->IsNull()) {
		return scope.Close(ThrowException(Exception::Error(String::New("Key must be an Object"))));
	}
	Handle<Object> obj = key->ToObject();
	EphemeronMap* mymap = UnwrapMap(args.This());
	EphemeronMap::iterator i = mymap->find(obj->GetIdentityHash());
	if(i != mymap->end()) {
		EphemeronVector* vector = (*i).second;
		EphemeronVector::iterator ii = vector->begin();
		while(ii != vector->end()) {
			EphemeronPair pair = *ii;
			if((*pair.first)->StrictEquals(obj)) {
				return scope.Close(*pair.second);
			}
			ii++;
		}
		i++;
	}
	return scope.Close(Undefined());
}

Handle<Value> EphemeronHas(
		const Arguments& args
	) {
	HandleScope scope;
	Handle<Value> key = args[0];
	if (key->IsUndefined() || key->IsNull()) {
		return scope.Close(ThrowException(Exception::Error(String::New("Key must be an Object"))));
	}
	Handle<Object> obj = key->ToObject();
	EphemeronMap* mymap = UnwrapMap(args.This());
	EphemeronMap::iterator i = mymap->find(obj->GetIdentityHash());
	if(i != mymap->end()) {
		EphemeronVector* vector = (*i).second;
		EphemeronVector::iterator ii = vector->begin();
		while(ii != vector->end()) {
			if((*(*ii).first)->StrictEquals(obj)) {
				return scope.Close(Boolean::New(true));
			}
			ii++;
		}
		i++;
	}
	return scope.Close(Boolean::New(false));
}

Handle<Value> EphemeronKeys(
		const Arguments& args
	) {
	HandleScope scope;
	EphemeronMap* mymap = UnwrapMap(args.This());
	EphemeronMap::iterator i = mymap->begin();
	int j = 0;
	Handle<Array> keys = Array::New(mymap->size());
	while(i != mymap->end()) {
		EphemeronVector* vector = (*i).second;
		EphemeronVector::iterator ii = vector->begin();
		while(ii != vector->end()) {
			keys->Set(j,*((*ii).first));
			ii++;
			j++;
		}
		i++;
	}
	return scope.Close(keys);
}
