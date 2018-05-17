const templates = [`z,c,v,n,8,r,0,3,p,2,y,t,o
d,b,x,e,f,g,h,i,j,k,l,m,t`,
                   `0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
c,b,s,j,0,p,u,f,4,a,7,6,q,d,r,e,t,2,n,3,l,1,9,k,g,m,y,5,o,i,w,z,8,x,h,v`];

/**
 * Decrypt Method
 *
 * @param {any} value
 * @param {any} mappingNumber
 */
exports.decrypt = (value, mappingNumber) => {
  try {
    let [chars, encryptedChars] = templates[mappingNumber - 1].split('\n').map(row => row.split(','))

    const charObject = chars.reduce((a, value, key) => {
      return Object.assign(a, {
        [value]: encryptedChars[key]
      }, (isNaN(parseInt(value)) ? {
        [value.toUpperCase()]: encryptedChars[key].toUpperCase()
      } : {}));
    }, {});

    return Array.from(value).map(char => charObject[char] || char).join('')
  } catch(err) {
    return false;
  }
}
