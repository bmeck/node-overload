#Node-Overload

##install

####If you have npm
1. npm install overload

####Raw
1. run __make__
2. files are in lib directory

##exports

####overload.node

* Watchable(onGet,onSet,onForeach,onQuery,onDelete)

	Callback's can use the __this__ object in order to act normally without reinvoking themselves.

* * Value onGet(ArgInfo) - property

	Returns the value at a specific index
* * Value onSet(ArgInfo) - property, value

	Returns the value to save at a specific index
* * Array onForeach(ArgInfo)

	Returns an array containing all the index keys for this object
* * Boolean onQuery(ArgInfo) - property

	Returns whether the object has a specific index
* * Boolean onDelete(ArgInfo) - property

	Returns whether the object was successful in deleting a specific index

* * ArgInfo Values - holder thisObject property? value?

* RePrototype(obj,proto)

	Set obj's prototype to proto

####proxy.js

Partial implementation of ES Harmony Proxy API.

####utils.js

* ReadOnly(obj,mask)

	Prevent an object from being modified w/ a setter. Methods on this object may still modify the object.

* * mask - list of properties to intercept properties (useful to replace methods you dont want to modify the object)

* * ReadOnly.ArrayMask

* * ReadOnly.DateMask

##example

####Code
	var proxied={hello:""}
	var debug = Watchable(
		//GETTER
		function(ArgInfo){
			//check the type of the object contained by value
			//undefined if no value, or number if using []'s with a number
			var property=ArgInfo.property
			sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]));
			return proxied[property];
		}
		//SETTER
		, function(ArgInfo) {
			var property=ArgInfo.property
			var value=ArgInfo.value
			sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]),"NEW VALUE:"+String());
			pro
			return ArgInfo.value;
		}
	)
	debug.hello
	debug[" world!"]=true

####Output

	GET:"hello"
	VALUE:
	SET:" world!"
	VALUE:undefined
	VALUE:true

##uses

* Watchable
1. Debugging - Show what is being accessed / set
2. False natives - refuse to allow properties to be set / got beyond a specified few
3. Dynamic programming - Fib[3] could compute Fib[1] and Fib[2]
4. Index based getters / setters - Fib[0] cannot have a getter set normally because it is a number not string based index (all non-number non-undefined values become string based).

* RePrototype
1. Making Watchables follow the instanceof operator
2. Making extensions of Objects follow the instanceof operator

* ReadOnly
1. Making arrays that cant be messed with