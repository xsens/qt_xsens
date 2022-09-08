// Copyright 2017 Mathias Bynens. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
author: Mathias Bynens
description: >
  Unicode property escapes for `Script=Balinese`
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
    [0x001B00, 0x001B4B],
    [0x001B50, 0x001B7C]
  ]
});
testPropertyEscapes(
  /^\p{Script=Balinese}+$/u,
  matchSymbols,
  "\\p{Script=Balinese}"
);
testPropertyEscapes(
  /^\p{Script=Bali}+$/u,
  matchSymbols,
  "\\p{Script=Bali}"
);
testPropertyEscapes(
  /^\p{sc=Balinese}+$/u,
  matchSymbols,
  "\\p{sc=Balinese}"
);
testPropertyEscapes(
  /^\p{sc=Bali}+$/u,
  matchSymbols,
  "\\p{sc=Bali}"
);

const nonMatchSymbols = buildString({
  loneCodePoints: [],
  ranges: [
    [0x00DC00, 0x00DFFF],
    [0x000000, 0x001AFF],
    [0x001B4C, 0x001B4F],
    [0x001B7D, 0x00DBFF],
    [0x00E000, 0x10FFFF]
  ]
});
testPropertyEscapes(
  /^\P{Script=Balinese}+$/u,
  nonMatchSymbols,
  "\\P{Script=Balinese}"
);
testPropertyEscapes(
  /^\P{Script=Bali}+$/u,
  nonMatchSymbols,
  "\\P{Script=Bali}"
);
testPropertyEscapes(
  /^\P{sc=Balinese}+$/u,
  nonMatchSymbols,
  "\\P{sc=Balinese}"
);
testPropertyEscapes(
  /^\P{sc=Bali}+$/u,
  nonMatchSymbols,
  "\\P{sc=Bali}"
);
