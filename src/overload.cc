

#include "watchable.cc"
#include "ephemeron.cc"

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	////printf("init\n");
	Local<FunctionTemplate> watchable_template = FunctionTemplate::New(Watchable);
	Local<Function> watchable = watchable_template->GetFunction();
	//Export
	target->Set(String::New("Watchable"),watchable);

	Local<FunctionTemplate> reprototype_template = FunctionTemplate::New(RePrototype);
	Local<Function> reprototype = reprototype_template->GetFunction();
	//Export
	target->Set(String::New("RePrototype"),reprototype);

	Local<FunctionTemplate> weak_template = FunctionTemplate::New(Weak);
	Local<Function> weak = weak_template->GetFunction();
	//Export
	target->Set(String::New("OnCollect"),weak);

	Handle<FunctionTemplate> table_template = FunctionTemplate::New(EphemeronInit);
	Handle<Template> table_prototype_template = table_template->PrototypeTemplate();
	table_prototype_template->Set("set",FunctionTemplate::New(EphemeronSet));
	table_prototype_template->Set("get",FunctionTemplate::New(EphemeronGet));
	table_prototype_template->Set("has",FunctionTemplate::New(EphemeronHas));
	table_prototype_template->Set("keys",FunctionTemplate::New(EphemeronKeys));

	table_instance_template = Persistent<ObjectTemplate>::New(table_template->InstanceTemplate());
	table_instance_template->SetInternalFieldCount(1);

	Handle<Function> table = table_template->GetFunction();
	target->Set(String::New("EphemeronTable"),table);

	scope.Close(Undefined());
}
