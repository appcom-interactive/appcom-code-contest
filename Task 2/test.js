// Replace `template` with your own solution
const fs = require('fs');
const path = require('path');
const mocha = require('mocha');
const expect = require('chai').expect;

const it = mocha.it;
const describe = mocha.describe;

const files = fs.readdirSync(path.resolve(__dirname, './solutions'))
  .filter(file => file !== 'template.js' && file !== '.DS_Store' && !fs.lstatSync(path.resolve(__dirname, './solutions', file)).isDirectory())
  .map(file => ({
    name: file.replace('.js', ''),
    impl: require(path.resolve('./solutions', file))
  }));

files.forEach(({
  name,
  impl
}) => {
  describe(`${name} solution`, () => {
    it('should return the decrypted value for Rn0n3tonvo?', () => {
      expect(impl.decrypt('Rn0n3tonvo?', 1)).to.eq('Geheimtext?');
    });

    it('should return the decrypted value for abCDefgh', () => {
      expect(impl.decrypt('abCDefgh', 2)).to.eq('76QDret2');
    });

    it('should return the decrypted value for appCoM', () => {
      expect(impl.decrypt('appCoM', 2)).to.eq('7mmQg9');
    });

    it('should return the decrypted value for KäsE', () => {
      expect(impl.decrypt('KäsE', 2)).to.eq('LäoR');
    });

    it('should return the decrypted value for KeKsDOSe', () => {
      expect(impl.decrypt('KeKsDOSe', 1)).to.eq('KeKsDTSe');
    });
  });
})