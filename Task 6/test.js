const fs = require('fs');
const path = require('path');
const mocha = require('mocha');
const expect = require('chai').expect;

const it = mocha.it;
const describe = mocha.describe;

const files = fs
  .readdirSync(path.resolve(__dirname, './solutions'))
  .filter(
    file =>
      file !== 'template.js' &&
      file !== '.DS_Store' &&
      !fs.lstatSync(path.resolve(__dirname, './solutions', file)).isDirectory()
  )
  .map(file => ({
    name: file.replace('.js', ''),
    impl: require(path.resolve('./solutions', file))
  }));

files.forEach(({ name, impl }) => {
  describe(`${name} solution`, () => {
    it('', () => {});
  });
});
