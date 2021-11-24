#include "equipArray.h"

struct equipArray_s {
	int equipaggiamento[EQUIP_SLOT];
	int in_uso;
};

equipArray_t equipArray_init() {
	int i;
	equipArray_t equip = (equipArray_t)malloc(sizeof(struct equipArray_s));
	if (equip != NULL) {
		equip->in_uso = 0;
		for (i = 0; i < EQUIP_SLOT; i++)
			equip->equipaggiamento[i] = -1;
	}
	return equip;
}

void equipArray_free(equipArray_t equipArray) {
	free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
	return equipArray->in_uso;
}

void equipArray_print(FILE* fp, equipArray_t equipArray, invArray_t invArray) {
	int i;
	for (i = 0; i < EQUIP_SLOT; i++)
		if (equipArray->equipaggiamento[i] != -1)
			invArray_printByIndex(fp, invArray, equipArray->equipaggiamento[i]);
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
	int i = -1, inv_index;
	int scelta;
	char nome[LEN];

	do {
		printf("  0 > Aggiungere equipaggiamento\n\t1 > Rimuovere equipaggiamento\n");
		scanf(" %d", &scelta);
		getchar();
	} while (scelta != 1 && scelta != 0);

	if (scelta == 0) {
		if (equipArray->in_uso < EQUIP_SLOT) {
			printf("Inserire il nome dell'oggetto da equipaggiare: ");
			scanf("%s", nome);
			for (i = 0; i < EQUIP_SLOT; i++) {
				if (equipArray->equipaggiamento[i] == -1) {
					equipArray->equipaggiamento[i] = invArray_searchByName(invArray, nome);
					break;
				}
			}
		}
	}
	else {
		printf("Inserire il nome dell'oggetto da rimuovere: ");
		scanf("%s", nome);
		inv_index = invArray_searchByName(invArray, nome);
		for (i = 0; i < EQUIP_SLOT; i++) {
			if (equipArray->equipaggiamento[i] == inv_index) {
				equipArray->equipaggiamento[i] = -1;
				break;
			}
		}
	}
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
	return (index >= 0 && index < EQUIP_SLOT) ? equipArray->equipaggiamento[index] : -1;
}

stat_t equipArray_getStatModifiers(equipArray_t equipArray, invArray_t invArray) {
	stat_t modifiers, tmp;
	int i;
	modifiers.atk = modifiers.def = modifiers.hp = modifiers.mp = modifiers.mag = modifiers.spr = 0;
	for (i = 0; i < EQUIP_SLOT; i++) {
		if (equipArray->equipaggiamento[i] != -1) {
			tmp = inv_getStat(invArray_getByIndex(invArray, equipArray->equipaggiamento[i]));
			modifiers.atk += tmp.atk;
			modifiers.def += tmp.def;
			modifiers.hp += tmp.hp;
			modifiers.mp += tmp.mp;
			modifiers.mag += tmp.mag;
			modifiers.spr += tmp.spr;
		}
	}
	return modifiers;
}
