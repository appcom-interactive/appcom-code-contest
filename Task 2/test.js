// Replace `template` with your own solution
const substitution = require('./solutions/rene');

var mocha = require('mocha');
var describe = mocha.describe;
var it = mocha.it;

const expect = require('chai').expect;

describe('your substitution solution', () => {
  it('should return the decrypted value for Rn0n3tonvo?', () => {
    expect(substitution.decrypt('Rn0n3tonvo?', 1)).to.eq('Geheimtext?');
  });

  it('should return the decrypted value for abCDefgh', () => {
    expect(substitution.decrypt('abCDefgh', 2)).to.eq('76QDret2');
  });

  it('should return the decrypted value for appCoM', () => {
    expect(substitution.decrypt('appCoM', 2)).to.eq('7mmQg9');
  });

  it('should return the decrypted value for KäsE', () => {
    expect(substitution.decrypt('KäsE', 2)).to.eq('LäoR');
  });

  it('should return the decrypted value for KeKsDOSe', () => {
    expect(substitution.decrypt('KeKsDOSe', 1)).to.eq('KeKsDTSe');
  });
});
