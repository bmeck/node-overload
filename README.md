#Node-Overload

##install

####If you have [npm](http://github.com/isaacs/npm)
> npm install overload

####Raw
>  make

Files are in __lib__ directory

##exports

####overload.node

> ##Watchable(onGet,onSet,onForeach,onQuery,onDelete)
> Callback's can use the __this__ object in order to act normally without reinvoking themselves.
>
> * Value onGet(ArgInfo) - property

>	Returns the value at a specific index
> * Value onSet(ArgInfo) - property, value

>	Returns the value to save at a specific index
> * Array onForeach(ArgInfo)

>	Returns an array containing all the index keys for this object
> * Boolean onQuery(ArgInfo) - property

>	Returns whether the object has a specific index
> * Boolean onDelete(ArgInfo) - property

>	Returns whether the object was successful in deleting a specific index
> * ArgInfo Values - holder thisObject property? value?
>
> ## RePrototype(obj,proto)
>
>	Set obj's prototype to proto

> ##OnCollect(Object,collector)
> Calls the collector when the Object is about to be Garbage Collected

> * undefined collector(theObject)

> ##new EphemeronTable()
> Creates an ephemeron table that only keeps its keys until that key is garbage collected

> * Value get(Object)
> * Value set(Object,Value)
> * Boolean has(Object)
> * Object[] keys()

####proxy.js

Partial implementation of [ES Harmony Proxy API](http://wiki.ecmascript.org/doku.php?id=harmony:proxies&s=proxy).

####weakmap.js

Implementation of ES [Harmony WeakMap API](http://wiki.ecmascript.org/doku.php?id=harmony:weak_maps).

####weakpointer.js

Implementation of [ES Harmony WeakPointer API](http://wiki.ecmascript.org/doku.php?id=strawman:weak_references).

####utils.js

> ##ReadOnly(obj,mask)
>
>	Prevent an object from being modified w/ a setter. Methods on this object may still modify the object.
>
> * mask - list of properties to intercept properties (useful to replace methods you dont want to modify the object)
> * ReadOnly.ArrayMask - prevents Array Object from being modified by methods
> * ReadOnly.DateMask - prevent Date Object from being modified by methods

##example

####Code
	var Watchable = require( "../lib/overload" ).Watchable
	  , proxied={ hello : "" }
	  , debug = Watchable(
		function getter( ArgInfo ){
			//check the type of the object contained by value
			//undefined if no value, or number if using []'s with a number
			var property = ArgInfo.property
			assert.equal( property, "hello", "GETTER PROPERTY NAME" )
			console.log( "GET:" + JSON.stringify( property ) )
			console.log( "VALUE:" + String(proxied[ property ] ) )
			return proxied[ property ];
		}
		, function setter( ArgInfo ) {
			var property = ArgInfo.property
			assert.equal( property, " world!", "SETTER PROPERTY NAME" )
			var value = ArgInfo.value
			assert.equal( value, true, "SETTER PROPERTY VALUE" )
			console.log( "GET:"+JSON.stringify( property ) )
			console.log( "VALUE:"+String( proxied[ property ] ) )
			console.log( "NEW VALUE:" + String( value ) )
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
	NEW VALUE:true

##uses

#### Watchable
> 1. Debugging - Show what is being accessed / set
> 2. False natives - refuse to allow properties to be set / got beyond a specified few
> 3. Dynamic programming - Fib[3] could compute Fib[1] and Fib[2]
> 4. Index based getters / setters - Fib[0] cannot have a getter set normally because it is a number not string based index (all non-number non-undefined values become string based).

#### RePrototype
> 1. Making Watchables follow the instanceof operator
> 2. Making extensions of Objects follow the instanceof operator

#### ReadOnly
> 1. Making arrays that cant be messed with

#### TODO

> 1. EphemeronTable is too generous in allocating memory for it's hash table.