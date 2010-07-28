var ReadOnly = require("../lib/util").ReadOnly
  , assert = require("assert")
var a = []
  , ra = ReadOnly(a,ReadOnly.ArrayMask,ReadOnly.ArrayFilter)
ra.push(1)
assert.equal(a.length,0,"push")
ra[0]=2
assert.equal(a.length,0,"index")
assert.equal(a[0],null,"get")
a[0]=1
assert.equal(ra.length,1,"index")
