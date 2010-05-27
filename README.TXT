node-watchable
	@exports
		Watchabe(onGet,onSet) - Supplies getter and setter callbacks across all the properties

	@example
		//print out any property gets that use the . operator
		var debug = Watchable(
			function(propertyName,value,hadAlready){
				//check the type of the object contained by value
				//undefined if no value, or number if using []'s with a number
				if(typeof value === "string") {
					sys.puts(propertyName);
				}
				return value;
			}
			function(propertyName,oldValue,value,hadAlready) {
				return value;
			}
		)