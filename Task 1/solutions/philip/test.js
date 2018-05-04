const Polybios = require('./index');

var mocha = require('mocha');
var describe = mocha.describe;
var it = mocha.it;

const expect = require('chai').expect;

const poly = new Polybios(5);

const hallo = poly.encrypt('HALLO');
const appcom = poly.encrypt('APPCOM');
const bat = poly.encrypt('BAT');
const flugzeug = poly.encrypt('FLUGZEUG');
const webdev = poly.encrypt('WEBDEV');
const internet = poly.encrypt('INTERNET');

describe('your polybios solution encrypt', () => {
    it('should return the encrypted value for HALLO', () => {
        expect(hallo).to.eq('23 11 31 31 34');
    });

    it('should return the encrypted value for APPCOM', () => {
        expect(appcom).to.eq('11 35 35 13 34 32');
    });

    it('should return the encrypted value for BAT', () => {
        expect(bat).to.eq('12 11 44');
    });

    it('should return the encrypted value for FLUGZEUG', () => {
        expect(flugzeug).to.eq('21 31 45 22 55 15 45 22');
    });

    it('should return the encrypted value for WEBDEV', () => {
        expect(webdev).to.eq('52 15 12 14 15 51');
    });

    it('should return the encrypted value for INTERNET', () => {
        expect(internet).to.eq('24 33 44 15 42 33 15 44');
    });
});

describe('your polybios solution decrypt', () => {
    it('should return the decrypted value for HALLO', () => {
        expect(poly.decrypt(hallo)).to.eq('HALLO');
    });

    it('should return the decrypted value for APPCOM', () => {
        expect(poly.decrypt(appcom)).to.eq('APPCOM');
    });

    it('should return the decrypted value for BAT', () => {
        expect(poly.decrypt(bat)).to.eq('BAT');
    });

    it('should return the decrypted value for FLUGZEUG', () => {
        expect(poly.decrypt(flugzeug)).to.eq('FLUGZEUG');
    });

    it('should return the decrypted value for WEBDEV', () => {
        expect(poly.decrypt(webdev)).to.eq('WEBDEV');
    });

    it('should return the decrypted value for INTERNET', () => {
        expect(poly.decrypt(internet)).to.eq('INTERNET');
    });
});