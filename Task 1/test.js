// Replace `template` with your own solution
const polybios = require('./solutions/template');

var mocha = require('mocha');
var describe = mocha.describe;
var it = mocha.it;

const assert = require('assert');

describe('your polybios solution', () => {
    it('should return the decrypted value for HALLO', () => {
        assert(polybios.encrypt('HALLO'), '23 11 31 31 34');
    });

    it('should return the decrypted value for APPCOM', () => {
        assert(polybios.encrypt('APPCOM'), '11 35 35 13 34 32');
    });
    
    it('should return the decrypted value for BAT', () => {
        assert(polybios.encrypt('BAT'), '11 35 35 13 34 32');
    });
    
    it('should return the decrypted value for FLUGZEUG', () => {
        assert(polybios.encrypt('FLUGZEUG'), '21 31 45 22 55 15 45 22');
    });

    it('should return the decrypted value for WEBDEV', () => {
        assert(polybios.encrypt('WEBDEV'), '52 15 12 14 15 51');
    });

    it('should return the decrypted value for INTERNET', () => {
        assert(polybios.encrypt('INTERNET'), '24 33 44 15 42 33 15 44');
    });
});