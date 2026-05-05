# 🌌 XOR-Linked List: Киберпанк в мире структур данных

[![Swag Level: Maximum](https://img.shields.io/badge/Swag-Maximum-blueviolet?style=for-the-badge)](https://github.com/)
[![Memory: Optimized](https://img.shields.io/badge/Memory-Optimized-success?style=for-the-badge)](https://github.com/)
[![Pointer Magic: 100%](https://img.shields.io/badge/Pointer_Magic-100%25-ff69b4?style=for-the-badge)](https://github.com/)
[![Warning: Segfaults](https://img.shields.io/badge/Warning-Segfaults_Guaranteed-red?style=for-the-badge)](https://github.com/)

Хранить два указателя (`next` и `prev`) в узле двусвязного списка — это скучный мейнстрим. Зачем тратить драгоценную память, если можно использовать темную магию побитовых операций и сжать два адреса в один? 

Этот репозиторий — реализация **XOR Linked List**. Ультимативный флекс низкоуровневым кодом, где указатели сливаются воедино через `^`. 

---

## ⚡ Фичи

* 🧠 **Brain-bending:** Ломает привычное восприятие памяти. Идеально для того, чтобы впечатлить (или напугать) коллег.
* 💾 **Eco-friendly Memory:** Экономит целых 8 байт на каждый узел (на 64-битных системах). Мы бережем оперативку так, как будто на дворе снова 90-е.
* 🚀 **Blazing Fast:** Битовые операции `XOR` выполняются буквально на кончиках пальцев процессора.
* 🕶️ **100% C Vibe:** Чистая работа с адресами, `uintptr_t` и кастомными аллокациями. Никаких сборщиков мусора, только хардкор.

---

## 🛠️ Как это вообще работает?

Обычный узел хранит `Node* prev` и `Node* next`. 
Наш свэговый узел хранит только `XORNode *nb`.

`nb` — это результат побитового исключающего ИЛИ (XOR) между адресами предыдущего и следующего узлов:
`nb = prev ^ next`

Когда ты идешь по списку слева направо, у тебя в руках есть адрес текущего узла и адрес предыдущего. Чтобы получить следующий, ты просто делаешь:
`next = prev ^ current->nb`

*Да, это так просто. И да, это выглядит незаконно.*

---

## 💻 Quick Start (Погружение в Матрицу)

Вот как выглядит ядро нашего движка под капотом:

```c
// Базовая структура
struct XORNode {
	int data;
	struct XORNode *nb; // nb - neighbors
};				

// Главное заклинание репозитория
static struct XORNode *XOR(struct XORNode *pt1, struct XORNode *pt2) {
	return (void *) ((uintptr_t) pt1 ^ (uintptr_t) pt2);
}