// Copyright 2009 the Sputnik authors.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
info: Operator remove leading StrWhiteSpaceChar
es5id: 15.1.2.3_A2_T8
es6id: 18.2.4
esid: sec-parsefloat-string
description: "StrWhiteSpaceChar :: LS (U+2028)"
---*/

//CHECK#1
if (parseFloat("\u20281.1") !== parseFloat("1.1")) {
  $ERROR('#1: parseFloat("\\u20281.1") === parseFloat("1.1"). Actual: ' + (parseFloat("\u20281.1")));
}

//CHECK#2
if (parseFloat("\u2028\u2028-1.1") !== parseFloat("-1.1")) {
  $ERROR('#2: parseFloat("\\u2028\\u2028-1.1") === parseFloat("-1.1"). Actual: ' + (parseFloat("\u2028\u2028-1.1")));
}

//CHECK#3
assert.sameValue(parseFloat("\u2028"), NaN);
