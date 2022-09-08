// Copyright 2017 Mathias Bynens. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
author: Mathias Bynens
description: >
  Unicode property escapes for `Script=Imperial_Aramaic`
info: |
  Generated by https://github.com/mathiasbynens/unicode-property-escapes-tests
  Unicode v10.0.0
  Emoji v5.0 (UTR51)
esid: sec-static-semantics-unicodematchproperty-p
features: [regexp-unicode-property-escapes]
includes: [regExpUtils.js]
---*/

const matchSymbols = buildString({
  loneCodePoints: [],
  ranges: [
    [0x010840, 0x010855],
    [0x010857, 0x01085F]
  ]
});
testPropertyEscapes(
  /^\p{Script=Imperial_Aramaic}+$/u,
  matchSymbols,
  "\\p{Script=Imperial_Aramaic}"
);
testPropertyEscapes(
  /^\p{Script=Armi}+$/u,
  matchSymbols,
  "\\p{Script=Armi}"
);
testPropertyEscapes(
  /^\p{sc=Imperial_Aramaic}+$/u,
  matchSymbols,
  "\\p{sc=Imperial_Aramaic}"
);
testPropertyEscapes(
  /^\p{sc=Armi}+$/u,
  matchSymbols,
  "\\p{sc=Armi}"
);

const nonMatchSymbols = buildString({
  loneCodePoints: [
    0x010856
  ],
  ranges: [
    [0x00DC00, 0x00DFFF],
    [0x000000, 0x00DBFF],
    [0x00E000, 0x01083F],
    [0x010860, 0x10FFFF]
  ]
});
testPropertyEscapes(
  /^\P{Script=Imperial_Aramaic}+$/u,
  nonMatchSymbols,
  "\\P{Script=Imperial_Aramaic}"
);
testPropertyEscapes(
  /^\P{Script=Armi}+$/u,
  nonMatchSymbols,
  "\\P{Script=Armi}"
);
testPropertyEscapes(
  /^\P{sc=Imperial_Aramaic}+$/u,
  nonMatchSymbols,
  "\\P{sc=Imperial_Aramaic}"
);
testPropertyEscapes(
  /^\P{sc=Armi}+$/u,
  nonMatchSymbols,
  "\\P{sc=Armi}"
);
