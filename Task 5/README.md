# TEMCO® Industries Drill Checker

## appcom code contest Task 5

Die Firma TEMCO® Industries, ein führendes Unternehmen in der Erdölförderung hat einen Auftrag für Dich. Sie haben auf ihrem Gelände mehrere Ölquellen nur wenige Meter unter dem Boden gefunden und wollen die Förderung dieser nun auf Wirtschaftlichkeit prüfen lassen.
Ein Drittunternehmen hat für 4 Grundstücke den Schichtaufbau des Erdreichs analysiert und dir die Pläne digital zukommen lassen.

Die Pläne werden wie folgt gelesen:

```javascript
// Schicht 1
 [
   ['E', 'E', 'E', 'E', 'E'], ^
   ['E', 'E', 'E', 'E', 'E'], |
   ['E', 'E', 'E', 'E', 'E'], Z+
   ['E', 'E', 'E', 'E', 'E'], |
   ['E', 'E', 'E', 'E', 'E'], |
// ---------- X+ ---------->
 ],
// Schicht 2
 [
   ['E', 'E', 'E', 'E', 'E'],
   ['E', 'G', 'G', 'E', 'S'],
   ['E', 'G', 'G', 'K', 'S'],
   ['S', 'E', 'K', 'K', 'S'],
   ['E', 'S', 'E', 'S', 'E'],
 ]
```
```
Die verschiedenen Erdreichtypen (Auszug aus dem TEMCO® Bohrverwaltungstool® V1.0, Stand: Feb. 2018):
+------------------+----------------+-----------------------+
| Kürzel (im Plan) | Name           | Bohrdauer pro m³ in h |
+------------------+----------------+-----------------------+
| E                | Erde           | 1,5                   |
| H                | Hohlraum       | 0,5                   |
| K                | Kiesel         | 1,0                   |
| S                | Stein          | 2,5                   |
| L                | Lehm           | 1,5                   |
| G                | Granit         | 5,0                   |
+------------------+----------------------------------------+
```
Es soll nun die wirtschaftlichste Bohrstelle gefunden werden (geringster Zeitaufwand), dabei ist zu beachten, dass TEMCO Industries entschieden hat, die max. Bohrdauer auf maximal 10h zu begrenzen, da die Ölquellen nicht besonders groß sind und alles darüber hinaus wieder unwirtschaftlich wäre.

Das Ergebnis erwartet die TEMCO® Industries Drill-API wie folgt:
```javascript
// { x: 1, z: 2, t: 4 } Bei einer wirtschaftlichen Ölquelle
// [{ x: 1, z: 2, t: 4 }, { x: 2, z: 4, t: 4 }] Bei mehreren wirtschaftlichen Ölquellen
// 'Unwirtschaftliches Gelände', oder
// 'Unvollständige Geländedaten'
```
*Hinweis*:

"x" und "z" sind nicht 0-basiert. "t" ist die Zeit die benötigt wird um die Ölquelle zu erreichen.
