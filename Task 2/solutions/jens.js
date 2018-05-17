 /*
        ZWEITE AUFGABE:

        Überlegt euch eine Struktur mit der ihr eine substituive Verschlüsselung darstellen könnt.


        BEIPSIEL:
        Mapping:   a,e,h,l,o,t,w
                   | | | | | | | 
                   g,p,h,r,a,s,e
        Eingabe: Hallo Welt!
        Ausgabe: Hgrra Eprs!


        Folgende Mappings sollen implementiert werden:
        Mapping1:  z,c,v,n,8,r,0,3,p,2,y,t,o
                   | | | | | | | | | | | | |
                   d,b,x,e,f,g,h,i,j,k,l,m,t

        Mapping2:  0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
                   | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | 
                   c,b,s,j,0,p,u,f,4,a,7,6,q,d,r,e,t,2,n,3,l,1,9,k,g,m,y,5,o,i,w,z,8,x,h,v
                
    */

const s = (val) => val.split(',');

const mappings = [
  {
    decrypted: s('z,c,v,n,8,r,0,3,p,2,y,t,o'),
    encrypted: s('d,b,x,e,f,g,h,i,j,k,l,m,t')
  },
  {
    decrypted: s('0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z'),
    encrypted: s('c,b,s,j,0,p,u,f,4,a,7,6,q,d,r,e,t,2,n,3,l,1,9,k,g,m,y,5,o,i,w,z,8,x,h,v')
  }
];

module.exports = {
  decrypt(value, mappingNumber) {
    const { decrypted, encrypted } = mappings[mappingNumber - 1];
    return value.split('').map((char) => {
      const upper = isNaN(char) && char === char.toUpperCase();
      const index = decrypted.indexOf(char.toLowerCase());
      return index === -1 ? 
        char : (upper ? encrypted[index].toUpperCase() : encrypted[index]);
    }).join('');
  }
};
