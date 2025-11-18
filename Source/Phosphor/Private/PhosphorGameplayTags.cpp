// copyright Paradise_NiseMono


#include "PhosphorGameplayTags.h"
#include "GameplayTagsManager.h"

FPhosphorGameplayTags FPhosphorGameplayTags::GameplayTags;

void FPhosphorGameplayTags::InitializeNativeGameplayTags()
{
	// Primary Attributes (主要屬性)
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
		); // 增加物理傷害
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
		); // 增加魔法傷害
	
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
		); // 增加護甲與護甲穿透
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
		); // 增加生命值

	/*Secondary Attributes (次要屬性)*/
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves block chance")
		); // 減少受到的傷害，提升格檔機率
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
		); // 忽略敵人護甲百分比，增加爆擊機率
	
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
		); // 機率將受到的傷害減半
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
		); // 機率造成雙倍傷害外加爆擊傷害加成
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
		); // 成功爆擊時額外增加的傷害
	
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
		); // 降低攻擊者對你造成爆擊的機率
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
		); // 每秒回復的生命值
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
		); // 每秒回復的法力值
	
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
		); // 可獲得的最大生命值
	
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
		); // 可獲得的最大法力值

	/*Meta Attributes*/
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("IncomingXP")
		);
	
	/*Vital Attributes*/
	//GameplayTags.Attributes_Vital_Health=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	//GameplayTags.Attributes_Vital_Mana=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.mana"));



	/*Input Actions*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input tag for left mouse button")
		);
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input tag for right mouse button")
		);
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input tag for 1 button")
		);
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input tag for 2 button")
		);
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input tag for 3 button")
		);
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input tag for 4 button")
		);
	
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input tag for Passive Ability 1")
		);
	
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input tag for Passive Ability 2")
		);

	/*Damage*/
	GameplayTags.Damage= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);

	/*Damage Types*/
	GameplayTags.Damage_Fire= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	
	GameplayTags.Damage_Lightning= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
		);
	
	GameplayTags.Damage_Arcane= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	
	GameplayTags.Damage_Physical= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);

	/*Resistance*/
	GameplayTags.Attributes_Resistance_Fire= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire")
		, FString("Resistance of Fire Damage Type")
		);
	
	GameplayTags.Attributes_Resistance_Lightning= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Lightning")
	, FString("Resistance of Lightning Damage Type")
	);
	
	GameplayTags.Attributes_Resistance_Arcane= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Arcane")
	, FString("Resistance of Arcane Damage Type")
	);
	
	GameplayTags.Attributes_Resistance_Physical= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Physical")
	, FString("Resistance of Physical Damage Type")
	);

	/*Debuffs*/
	GameplayTags.Debuff_Burn= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn")
		, FString("Debuff of Fire Damage")
		);
	
	GameplayTags.Debuff_Stun= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Stun")
	, FString("Debuff of Lightning Damage")
	);
	
	GameplayTags.Debuff_Arcane= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Arcane")
	, FString("Debuff of Arcane Damage")
	);
	
	GameplayTags.Debuff_Physical= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Physical")
	, FString("Debuff of Physical Damage")
	);

	GameplayTags.Debuff_Chance= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance")
		, FString("Debuff Chance")
		);
	
	GameplayTags.Debuff_Damage= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Damage")
	, FString("Debuff Damage")
	);
	
	GameplayTags.Debuff_Frequency= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Frequency")
	, FString("Debuff Frequency")
	);
	
	GameplayTags.Debuff_Duration= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Duration")
	, FString("Debuff Duration")
	);

	/*Map of Damage Type to Resistance*/
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,GameplayTags.Attributes_Resistance_Physical);

	/*Map of Damage Type to Debuff*/
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire,GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning,GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane,GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical,GameplayTags.Debuff_Physical);
	
	/*Effects*/
	GameplayTags.Effects_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("React of Hit")
		);

	/*Abilities*/
	GameplayTags.Abilities_Attack= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
		);
	
	GameplayTags.Abilities_Summon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
		);

	GameplayTags.Abilities_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("None Ability Tag")
		);

	/*Offensive Spells*/
	
	GameplayTags.Abilities_Fire_FireBolt= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
		);
	
	GameplayTags.Abilities_Lightning_Electrocute=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag")
		);

	GameplayTags.Abilities_Arcane_ArcaneShards=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("Arcane Shards Ability Tag")
		);

	/*Passive Spells*/
	GameplayTags.Abilities_Passive_HaloOfProtection=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"),
		FString("Halo Of Protection Ability Tag")
		);

	GameplayTags.Abilities_Passive_LifeSiphon=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"),
		FString("Life Siphon Ability Tag")
		);

	GameplayTags.Abilities_Passive_ManaSiphon=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"),
		FString("Mana Siphon Ability Tag")
		);
	
	GameplayTags.Abilities_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("HitReact Ability Tag")
		);
	
	GameplayTags.Abilities_Status_Eligible= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible State")
		);
	
	GameplayTags.Abilities_Status_Equipped= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped State")
		);
	
	GameplayTags.Abilities_Status_Locked= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked State")
		);
	
	GameplayTags.Abilities_Status_Unlocked= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked State")
		);
	
	GameplayTags.Abilities_Type_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
		);
	
	GameplayTags.Abilities_Type_Offensive= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Type Offensive")
		);
	
	GameplayTags.Abilities_Type_Passive= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive")
		);
	
	/*Cooldown*/
	GameplayTags.Cooldown_Fire_FireBolt= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag")
		);

	/*CombatSocket*/
	GameplayTags.CombatSocket_Weapon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
		);
	
	GameplayTags.CombatSocket_RightHand= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
		);
	
	GameplayTags.CombatSocket_LeftHand= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
		);
	
	GameplayTags.CombatSocket_Tail= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail")
		);

	/*Montages*/
	GameplayTags.Montage_Attack_1= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Attack1"));
	GameplayTags.Montage_Attack_2= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Attack2"));
	GameplayTags.Montage_Attack_3= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Attack3"));
	GameplayTags.Montage_Attack_4= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Attack4"));

	/*Player Tags*/
	GameplayTags.Player_Block_CursorTrace= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block Tracing under the Cursor"));
	
	GameplayTags.Player_Block_InputPressed= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input pressed callback for input"));

	GameplayTags.Player_Block_InputHeld= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input held callback for input"));

	GameplayTags.Player_Block_InputReleased= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input released	 callback for input"));
	
}
