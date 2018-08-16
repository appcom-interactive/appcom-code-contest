# Vier gewinnt checker

## appcom code contest Task 4

Schreibe ein Methoden um den Gewinner eines Vier Gewinnt Spiels zu ermitteln und ob geschummelt wurde.

Du findest im template bereits diese Methoden. Technisch werden die nach jedem Wurf ausgeführt.

Alle Tests sollten erfolgreich sein.

Vier gewinnt wird üblich mit einem 7x6 großen Grid gespielt. Spieler A beginnt immer! Der Status des Spiels wird duch ein multidimensionales Array dargestellt. Hier ist der Status bei Spielbeginn.

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null]
];
```

Untere Reihe im echten Spiel die Position game[5]

Die Zustände sind null, A, und B - wobei A und B den Stein des jeweiligen
Spielers repräsentiert.

Wenn Du Lust hast, entwickel ein eigene Darstellung.

## Testfälle sind:

A steckt finger im loch

```javascript
[
  [null, null, null, null, null, null, null],
  [null, A, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, B, null, null, null, null, null],
  [null, B, null, null, null, null, null],
  [null, A, A, null, null, null, null]
];
```

B hat verdeckt zwei geworfen

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, B, B, B, null, null, null],
  [null, A, A, B, A, B, null],
  [null, A, B, A, A, B, null]
];
```

der blöde Nachbarjunge wollte unbedignt mitspielen und wirf einen Mühlestein ein

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, B, B, C, null, null, null],
  [null, A, A, B, A, B, null],
  [null, A, B, A, A, B, null]
];
```

spieler A gewinnt

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, A, null, null],
  [null, B, B, A, B, null, null],
  [null, A, A, B, A, B, null],
  [null, A, B, A, A, B, null]
];
```

spieler A gewinnt

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, null, null, null, null],
  [null, null, null, null, A, null, null],
  [null, B, B, null, A, B, null],
  [null, A, A, B, A, B, null],
  [null, A, B, A, A, B, null]
];
```

spieler B gewinnt

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, B, null, null, null],
  [null, null, B, A, null, null, null],
  [A, B, B, A, A, B, null],
  [B, A, A, B, A, B, null],
  [A, A, B, A, A, B, null]
];
```

spieler A gewinnt

```javascript
[
  [null, null, null, null, null, null, null],
  [null, null, null, B, B, null, null],
  [null, null, B, A, A, A, A],
  [A, B, B, A, B, A, B],
  [B, A, A, B, A, B, A],
  [A, A, B, A, B, B, B]
];
```
