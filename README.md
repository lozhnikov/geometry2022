# geometry

Алгоритмы на вычислительную геометрию.

## О добавлении изменений в репозиторий.

**Любые изменения необходимо добавлять в проект при помощи Pull Request.** Огромная просьба: **не нажимать** кнопку `Merge` до тех пор, пока я не проверю код и не напишу, что можно делать Merge.

### Создание Pull Request.

Предположим, что пользователю **UserName** требуется сделать Pull Request. Для этого необходимо выполнить следующие действия:

1. Сделать Fork данного репозитория. Это делается на странице основного репозитория https://github.com/lozhnikov/geometry2022. Там есть кнопка Fork. Если уже сделан, то этот шаг пропускается. После этого будут доступны два независящих друг от друга репозитория с разными URL:
```
URL основного репозитория: https://github.com/lozhnikov/geometry2022
URL Fork'а:                https://github.com/UserName/geometry2022
```
2. Склонировать Ваш Fork на локальный компьютер. Если уже склонирован, то этот шаг пропускается. **Внимание.** Склонировать нужно именно Fork, а не основной репозиторий.
```bash
# Клонируем репозиторий. Перед клонированием нужно убедиться, что в текущей папке нет папки geometry2022.
git clone https://github.com/UserName/geometry2022
```
3. Как обычно вносим изменения в репозиторий, делаем коммит, выкладываем коммиты на сервер командой
```bash
# Выкладываем коммиты на сервер в Ваш Fork.
git push origin main
```
4. На странице Вашего Fork'а https://github.com/UserName/geometry2022 в Web интерфейсе необходимо нажать кнопку `New Pull Request`, и оформить Pull Request. Если коммиты выкладывались в нестандартную ветку (не `main`) или требуется выложить их в нестандартную ветку основного репозитория, то нужно это указать.

После создания Pull Request'а можно продолжать добавлять коммиты в ту ветку, из которой сделан Pull Request. Эти изменения появятся в самом Pull Request'е в Web интерфейсе.

Для того, чтобы сделать два Pull Request'а одновременно, необходимо их делать в разных ветках. В противном случае для создания нового Pull Request'а придётся сделать Merge старого. Вообще считается хорошим тоном для каждого Pull Request'а создавать новую ветку в своём Fork'е.

### Обновление Fork'нутых репозиториев.

В данный репозиторий переодически добавляются новые коммиты. Коммиты добавляю я, напрямую выкладывая их в ветку `main`, а также коммиты могут быть добавлены при помощи Pull Request (в этом случае происходит Merge из Вашей копии репозитория в данный репозиторий). Однако, при обновлении основного репозитория его Fork'и не обновляются, это нужно делать вручную.

Опишем проблему подробнее. Предположим, что пользователь **UserName** сделал Fork данного репозитория. В итоге получилось два независящих друг от друга репозитория с разными URL:
```
URL основного репозитория: https://github.com/lozhnikov/geometry2022
URL Fork'а:                https://github.com/UserName/geometry2022
```

Предположим, что в основной репозиторий добавились какие-то коммиты, и появилось желание перетащить их в Fork. Описываю процедуру по шагам. Для простоты предполагается, что изменения перетаскиваются из ветки `main` основного репозитория в ветку `main` Fork'а.

1. Необходимо склонировать Fork репозитория (если уже склонирован, то этот шаг нужно пропустить).
```bash
# Клонируем репозиторий. Перед клонированием нужно убедиться, что в текущей папке нет папки geometry2022.
git clone https://github.com/UserName/geometry2022
```
2. Переходим в корневую папку репозитория
```bash
cd geometry2022
```
3. Добавляем URL основного репозитория под именем `upstream` (можно использовать любое другое) в Git. **Эта команда выполняется один раз для каждого склонированного репозитория.** То есть её нужно выполнять только в том случае, если репозиторий был заново склонирован на Ваш локальный компьютер.
```bash
git remote add upstream https://github.com/lozhnikov/geometry2022
```
После добавления список удалённых репозиториев можно посмотреть командой
```bash
git remote -v
```
Эта команда выведет URL к репозиториям `origin` (Ваш Fork) и `upstream` (основной репозиторий). **Коммиты нужно по-прежнему добавлять в origin, репозиторий upstream используется только для чтения.**

4. Вытянуть с сервера коммиты основного репозитория.
```bash
# Эту команду нужно выполнять каждый раз. Спросит логин и пароль при доступе по https.
git fetch upstream
```

5. Сделать Merge ветки `main` основного репозитория в текущую ветку Вашего Fork'а (предполагается, что это тоже ветка `main`).
```bash
git merge upstream/main
```
6. Команда `git merge` может привести к конфликтам, которые нужно вручную исправить, добавить файлы с исправлениями при помощи `git add` и оформить в коммит. Если конфликтов нет, то этот шаг пропускается.

7. Забросить коммиты на сервер в Ваш Fork (в ветку `main`).
```bash
git push origin main
```

**Рекомендуется проводить эту процедуру почаще.** По сути, нужно повторять только 4-7 шаги. Рекомендуется это делать, как только в основном репозитории появятся новые коммиты.

### Создание нескольких Pull Request'ов одновременно

На GitHub можно создавать несколько Pull Request'ов одновременно, но для этого каждый Pull Request должен быть основан на своей собственной ветви Вашего Fork'а.

1. Сначала убедитесь, что Вы находитесь в ветки `main`. Новые ветви лучше делать на основе неё. Исходная ветвь не должна содержать коммитов, которые не смёрждены в основной репозиторий, иначе они появятся в Pull Request'е. Если она содержит несмердженные коммиты, то смотрите замечание в пункте 3.

```bash
# В репозитории не должно быть изменений, не оформленных в коммиты, иначе команда не сработает.
git checkout main
```

2. Создайте новую ветку командой `git branch`. Она создаёт ветвь на основе текущей ветви (`main`). Все коммиты, которые на данный момент есть в текущей ветви войдут в новую ветвь.

```bash
# Создаём ветвь BranchName на основе текущей (main).
git branch BranchName
```

3. Переключаемся на ветку `BranchName`

```bash
# В репозитории не должно быть незакоммиченных изменений.
git checkout BranchName
```

**Замечание**

В том случае, если исходная ветвь (`main`) содержала коммиты, которые не смёрджены в основной репозиторий, то в ветви `BranchName` они так же окажутся. Если Вы не хотите добавлять их в Pull Request, то их нужно удалить из ветви `BranchName`. Для этого после того, как Вы переключились на новую ветвь, нужно выполнить следующие команды:

a) Посмотрите ID последнего коммита, который нужно оставить. Для этого выполните команду 
```bash
git log
```
Эта команда покажет список коммитов и их ID (стоит после слова `commit`, представляет из себя шестнадцатеричную запись числа, например, `cc846edd7d59e839c5e246e11aa99b268441a111`). Для выхода из лога следует нажать клавишу `q` (убедитесь, что у Вас английская раскладка).

b) Удалите из текущей ветви (`BranchName`) все коммиты, сделанные после коммита с выбранным ID. Например,
```bash
# Команда удалит все коммиты, сделанные после коммита cc846edd7d59e839c5e246e11aa99b268441a111.
git reset --hard cc846edd7d59e839c5e246e11aa99b268441a111
```

4. Далее как обычно офрмляете коммиты. Они добавляются в текущую ветвь. Текущую ветвь можно сменить командой `git checkout`.

5. Для того, чтобы выложить коммиты ветки `BranchName` на сервер нужно выполнить команду

```bash
git push origin BranchName
```

6. При создании Pull Request'a нужно указать, из какой ветви Вы его создаёте.

7. Дополнение. Полный список ветвей можно посмотреть командой

```bash
git branch -a
```

## О стиле написания кода

Данная задача предназначена для того, чтобы смоделировать работу в реальной конторе или коллективе. Как правило, в приличных фирмах и сообществах принят единый стиль написания кода. Никто не станет добавлять Ваш код, если он написан так, что только Вы его понимаете. Поэтому, выполняя это задание, предстватьте, что Вы его делаете не для того, чтобы получить зачёт, а для того, чтобы сделать что-либо полезное, чем в дальнейшем смогут пользоваться другие люди.

Я предлагаю использовать что-то вроде [стиля написания кода от Google](https://google.github.io/styleguide/cppguide.html). У них есть специальная программа [cpplint](https://github.com/cpplint/cpplint), которая выдаёт предупреждения, когда видит нарушение стиля.

Приведу несколько примеров для тех, кому лень читать *google cpp style guide*.

**Длина каждой строчки не более 80 символов.** Это удобно, например, в тех случаях, когда требуется распечатать кусок кода.

**Размер одного отступа --- 2 пробела. Табуляции не используем.** Дело в том, что табуляция в разных текстовых редакторах представляется по-разному, поэтому при открытии в другом редакторе весь код может "поехать". В настоящее время практически любой текстовый редактор позволяет включить опцию "заменять табуляции пробелами". Поэтому данное требование не должно вызвать каких-либо проблем.

```c++
//  Типы данных называем осмысленными словами camel case'ом с большой буквы.
class CamelCase { //  Открывающаяся скобка там же, где объявление.
 //  Модификатор доступа на один пробел правее имени класса.
 private:
  //  Переменные camel case'ом с маленькой буквы (также осмысленными словами).
  double camelCaseVariable;

  void CamelCaseMethod() {
  }
 public:
  CamelCase() {
  }
};
```

Бинарные операторы рекомендуется отделять пробелами:
```c++
a = b + c;
c += d;
```

После `if`, `for`, `while` ставится пробел:
```c++
for (int i = 0; i < n; i++) {
}
```

## Детали выполнения задания

1. Добавляем в папку `include` файл `my_algorithm_name.hpp` (замените на название Вашего алгоритма snake case'ом). Реализовать в этом файле шаблонный алгоритм.
2. Добавить в папку `methods` файл `my_algorithm_name_method.cpp` (замените на название Вашего алгоритма snake case'ом).
3. Добавить в папку `tests` файл `my_algorithm_name_test.cpp` (замените на название Вашего алгоритма snake case'ом).
4. Созданные файлы нужно прописать в файл `CMakeLists.txt` **в алфавитном порядке**.
5. Реализовать в файле для метода серверную часть алгоритма, которая принимает на вход JSON, запускает алгоритм и записывает результат в выходной JSON. Метод должен запускаться одной функцией с прототипом `int MyAlgorithmNameMethod(const nlohmann::json& input, nlohmann::json* output)` (замените на название Вашего алгоритма camel case'ом). Прототип этой функции следует положить в файл `methods/methods.hpp`. Функция должна запускаться в обработчике на `POST` запрос `/MyAlgorithmName` (замените на название Вашего алгоритма camel case'ом), который нужно добавить в функцию `main()` в файле `methods/main.cpp`.
6. Реализовать набор тестов в созданном Вами файле для тестов. Он должен вызываться одной функцией с прототипом `void TestMyAlgorithmName()` (замените на название Вашего алгоритма camel case'ом), которую следует вызвать в функции `main()` в файле `tests/main.cpp`. Прототип этой функции следует положить в заголовочный файл `tests/test.hpp`. Требуется реализовать несколько статических тестов и хотя бы один тест со случайными значениями.
7. Написать документацию к коду. Добавить описание алгоритма и примеры его использования в файл `doc/algorithms/my_algorithm_name.dox`. Прописать документацию к алгоритму в списке алгоритмов в файле `doc/algorithms.dox`.

**Внимание.**
Для того, чтобы сразу отслеживать ошибки компиляции, а не после того, как уже всё написано, я советую добавить пустые заглушки для алгоритма и тестов. То есть Вы создаёте пустые функции, а реализацию дописываете позже. Добавьте самый простой тест, который просто запускает функцию с алгоритмом, но при этом ничего не проверяет. Это заставит компилятор компилировать шаблонную функцию с алгоритмом.

## Сборка проекта

Для сборки проекта нужно выполнить следующие команды:
```bash
# Создание папки для сборки. Лучше папку не переименовывать,
# поскольку она прописана в .gitignore. В данном примере
# предполагается, что папка build создаётся в корне репозитория.
mkdir build

# Перейти в папку для сборки.
cd build

# Сгенерировать Makefile при помощи cmake. Команде cmake необходимо
# передать путь к корню репозитория. В данном случае он находится на уровень выше.
cmake ../

# Запустить сборку.
make
```

При работе в Jupyter **для удобства разработки рекомендуется использовать компилятор clang** для того, чтобы избежать лишних warning'ов. Для этого нужно запустить `cmake` следующим образом:
```bash
cmake -D CMAKE_CXX_COMPILER=clang++ ../
```
Тем не менее, финальный вариант **должен компилироваться** компилятором `gcc` в Jupyter.

Кроме того, в Jupyter можно использовать `cpplint`. Для этого из корня репозитория нужно запустить следующую команду:
```
cpplint --recursive --filter="-legal/copyright,-build/c++11,-build/c++14,-build/include_subdir" --exclude=*/.ipynb_checkpoints/* methods/ tests/ include/
```

На некоторых системах cmake может сгенерировать файлы для другой системы сборки (не GNU Make). Тогда его нужно
запустить с ключом
```bash
cmake -G "Unix Makefiles" ../
```

Кроме того, могут быть определены следующие переменные, влияющие на сборку:
```bash
# Сгенерировать Makefile с включёнными флагами оптимизации.
cmake -D CMAKE_BUILD_TYPE=Release ../

# Сгенерировать Makefile для сборки проекта с отладочной информацией и без оптимизаций.
cmake -D CMAKE_BUILD_TYPE=Debug ../

# Сгенерировать Makefile для сборки проекта с включёнными предупреждениями компилятора.
# Это рекомендуемый способ, поскольку cmake по-умолчанию генерирует Makefile,
# в котором не все предупреждения включены.
cmake -D CMAKE_CXX_FLAGS="-Wall -Wextra" ../

# Переменные можно комбинировать.
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_FLAGS="-Wall -Wextra" ../
```

Команде `make` можно передать количество процессов, которые можно запустить одновременно, то есть сколько файлов можно одновременно компилировать. Рекомендуется выставлять `кол-во ядер процессора + 1`.
```
# Собрать проект, используя три процесса.
make -j3
```

## Запуск программы

После сборки программы в папке сборки появятся исполняемые файлы `geometry_server` и  `geometry_test`. Их нужно последовательно запустить в разных терминалах. Сначала сервер, а потом клиент.

```bash
# В первом терминале
./build/geometry_server

# Во втором терминале
./build/geometry_test
```

После успешного выполнения программы-клиента сервер прекращает работу.