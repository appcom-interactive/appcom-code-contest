// @flow
module.exports = {
    /**
     * Split string or an object
     *
     * @param {*} value
     * @param {?string} seperator
     * @returns {Array<mixed>}
     */
    split(value: any, seperator: ?string): Array<mixed> {
      // returned array
      const arr: Array<string | value> = [];
      let tempStr: string = '';

      // check if value is available
      if (value) {
        // check if entered value is a string
        if (typeof value === 'string') {
          if (!seperator) {
            arr.push(value);
          } else {
            /**
             * Get all words as array
             *
             * @param {*} value
             * @param {?string} seperator
             * @returns {Array<mixed>}
             */
            const getSplittedWords = (str: string, seperator: string): Array<mixed> => {
              // current index of seperator
              const currentIndex = str.indexOf(seperator);

              // get rest of string
              const restString = str.substr(currentIndex + 1);

              // get filtered word
              const word = str.substr(0, currentIndex);

              // check if seperator is at the end
              if (currentIndex + 1 === str.length) {
                return [word, ''];
              }

              // last item
              if (currentIndex === -1) {
                return [str];
              }

              // check if there is a rest
              if (restString && currentIndex >= 0) {
                return [word, ...getSplittedWords(restString, seperator)];
              }

              return [];
            };

            return getSplittedWords(value, seperator);


            // const r = '([^\/'+seperator+'$]+)';
            // const regex = new RegExp(r, 'g')

            // return value.match(regex);
            // // iterate through letters
            // for (let i = 0; i <= value.length; i++) {
            //   // add letter to tempStr, if its not the seperator
            //   if (value[i] && value[i] !== seperator) {
            //     tempStr += value[i];
            //   }

            //   // add tempStr to array, if char is the seperator
            //   // then the string is emptied
            //   if (!value[i] || value[i] === seperator) {
            //     arr.push(tempStr);
            //     tempStr = '';
            //   }
            // }
          }
        }
        // check if entered value is an object
        else if (typeof value === 'object') {
          for (const key: string in value) {
            arr.push({
              [key]: value[key]
            });
          }
        }
        // throw error, if value is not string or object
        else {
          throw Error('wrong input type');
        }
      }
      return arr;
    }
  };
