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
        it('should return the decrypted value of a,b,c,d,e', () => {
            expect(impl.split('a,b,c,d,e', ',')).to.deep.equal(['a', 'b', 'c', 'd', 'e']);
        });

        it('should return the splitted value of a;b;c;d;e', () => {
            expect(impl.split('a;b;c;d;e', ';')).to.deep.eq(['a', 'b', 'c', 'd', 'e']);
        });

        it('should return the splitted value of Hello World!', () => {
            expect(impl.split('Hello World!', ' ')).to.deep.eq(['Hello', 'World!']);
        });

        it('should return the splitted value of This is a longer sentence.', () => {
            expect(impl.split('This is a longer sentence.', '.')).to.deep.eq(['This is a longer sentence', '']);
            expect(impl.split('This is a longer sentence.', ' ')).to.deep.eq(['This', 'is', 'a', 'longer', 'sentence.']);
        });

        it('should return an empty array', () => {
            expect(impl.split(null, '.')).to.deep.eq([]);
            expect(impl.split(null, null)).to.deep.eq([]);
            expect(impl.split(null)).to.deep.eq([]);

            expect(impl.split(undefined, '.')).to.deep.eq([]);
            expect(impl.split(undefined, undefined)).to.deep.eq([]);
            expect(impl.split(undefined)).to.deep.eq([]);

            expect(impl.split('', '')).to.deep.eq([]);
            expect(impl.split('')).to.deep.eq([]);
            expect(impl.split()).to.deep.eq([]);
            expect(impl.split({})).to.deep.eq([]);
        });

        it('should return an array with input value', () => {
            expect(impl.split('Hello World!', 3214)).to.deep.eq(['Hello World!']);
            expect(impl.split('Hello World!', null)).to.deep.eq(['Hello World!']);
            expect(impl.split('Hello World!', undefined)).to.deep.eq(['Hello World!']);
        });

        it('should return the splitted value of {fkey: "one", skey: "two"}', () => {
            expect(impl.split({
                fkey: 'one',
                skey: 'two'
            })).to.deep.eq([{
                fkey: 'one'
            }, {
                skey: 'two'
            }]);
        });
    });
})