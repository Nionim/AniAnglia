<p align="center">
  <a href="#"><img width="100px" height="100px" src=".github/app_icon.png"></a>
</p>

<H1 align="center">-==[ AniSaturn - Fork of AniAnglia ]==-</H1>

<H2 align="center">-==[ Скачать ]==-</H2>
<p align="center">
    <a href="https://github.com/AniSaturn/AniSaturn/actions">Actions</a> или с 
    <a href="https://github.com/AniSaturn/AniSaturn/releases/latest">релизов</a> 
  <br><br>
  Установка выполняется через TrollStore/Sideloadly и подобные методы.
</p>

<H2 align="center">-==[ Собрать ]==-</H2>
<p align="center">
  Собрать можно двумя способами - Через форк и через git clone.
  <br> Через форк вам нужно будет просто зайти в Actions и запустить задачу сборки.
</p>

<h3>[ GitClone (MacOS only) >>></h3>

```bash
# UPD: В новейших версиях проект был перенесет на Cmake.
# UPD: Сбилдить его полностью через Xcode теперь нельзя. 
git clone https://github.com/AniSaturn/AniSaturn

# PROJECT_VERSION замените на требуемую вам версию
# PROJECT_BUILD - Аналогично
cmake -G Xcode -B build -DCMAKE_SYSTEM_NAME=iOS -DVER_SHORT="PROJECT_VERSION" -DVER_BUILD="PROJECT_BUILD" -DBUNDLE_ID="delta.cion.anisaturn"

# Тут ничего менять не надо, но можете покопаться при желании
xcodebuild -project build/AniSaturn.xcodeproj -scheme AniSaturn \
  -configuration Release \
  -sdk iphoneos -derivedDataPath build_derived \
  CODE_SIGNING_ALLOWED=NO CODE_SIGNING_REQUIRED=NO \
  PRODUCT_BUNDLE_IDENTIFIER="delta.cion.anisaturn"
```

<H2 align="center">-==[ For Devs ]==-</H2>

<p align="center">
    <a href="https://github.com/AniSaturn/AniSaturn_Closed_beta">Closed Beta Url</a>
	<br>
    <a href="https://github.com/AniSaturn/AniSaturn_Swift">Swift Port (Private repo)</a>
	<br>
    <a href="https://github.com/AniSaturn/AniSaturn?tab=contributing-ov-file">Contributing.md file</a> 
  <br><br>
	Если участвуете в разработке - Прошу - Вступите в дс/тг для связи. Контакты можно найти на профиле Nionim'а или в описании самой организации.
</p>


## TODO
Этот репозиторий создан чисто для удобства.
<br> Потом я возможно просто создам свой аналог.

```json
1. Переписать этот шедевр на Swift (Желательно сделать новое и с нуля)
2. Пересмотреть UI. Он мне не нравится
3. Кастомизация - Кастомные темы, цвета, сокрытие кнопочек
4. Спрятать всю китайскую ***** из списка с аниме
5. Бябябя Бебебе Бубубу
```

---

<p align="center">
    <a href="#">
        <img src="https://img.shields.io/github/last-commit/AniSaturn/AniSaturn_Swift?display_timestamp=committer&style=flat-square&color=000000"></a>   
    <a href="#">
        <img src="https://img.shields.io/github/created-at/AniSaturn/AniSaturn_Swift?style=flat-square&color=000000"></a>
</p>

