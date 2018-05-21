// Replace `template` with your own solution
const polybios = require('./solutions/jens');

var mocha = require('mocha');
var describe = mocha.describe;
var it = mocha.it;

const expect = require('chai').expect;

describe('your polybios solution', () => {
    it('should return the decrypted value for HALLO', () => {
        expect(polybios.encrypt('HALLO')).to.eq('23 11 31 31 34');
    });

    it('should return the decrypted value for APPCOM', () => {
        expect(polybios.encrypt('APPCOM')).to.eq('11 35 35 13 34 32');
    });

    it('should return the decrypted value for BAT', () => {
        expect(polybios.encrypt('BAT')).to.eq('12 11 44');
    });

    it('should return the decrypted value for FLUGZEUG', () => {
        expect(polybios.encrypt('FLUGZEUG')).to.eq('21 31 45 22 55 15 45 22');
    });

    it('should return the decrypted value for WEBDEV', () => {
        expect(polybios.encrypt('WEBDEV')).to.eq('52 15 12 14 15 51');
    });

    it('should return the decrypted value for INTERNET', () => {
        expect(polybios.encrypt('INTERNET')).to.eq('24 33 44 15 42 33 15 44');
    });
});