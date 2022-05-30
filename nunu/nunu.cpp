#include "../plugin_sdk/plugin_sdk.hpp"
#include "nunu.h"

namespace nunu {


#define Q_DRAW_COLOR (MAKE_COLOR ( 62, 129, 237, 255 ))  //Red Green Blue Alpha
#define E_DRAW_COLOR (MAKE_COLOR ( 235, 12, 223, 255 ))  //Red Green Blue Alpha
#define R_DRAW_COLOR (MAKE_COLOR ( 224, 77, 13, 255 ))   //Red Green Blue Alpha


	script_spell* q = nullptr;
    script_spell* w = nullptr;
    script_spell* e = nullptr;
    script_spell* r = nullptr;	


	script_spell* flash = nullptr;

	namespace draw_settings
    {
        TreeEntry* draw_range_q = nullptr;
        TreeEntry* draw_range_e = nullptr;
        TreeEntry* draw_range_r = nullptr;
        TreeEntry* draw_flash_range = nullptr;
    }

	namespace combo 
	{
		TreeEntry* use_q = nullptr;
		TreeEntry* use_e = nullptr;
		TreeEntry* use_r = nullptr;


	}

	namespace harass
	{
		TreeEntry* use_e = nullptr;

	}

	namespace jungleclear
	{
		TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;
	}

	// Event handler functions
    void on_update( );
    void on_draw( );
    void on_before_attack( game_object_script target, bool* process );

	void q_logic( );
    void e_logic( );
    void r_logic( );

	enum Position
    {
        Line,
        Jungle
    };

	Position my_hero_region;

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 125);
		e = plugin_sdk->register_spell(spellslot::q, 650);
		r = plugin_sdk->register_spell(spellslot::q, 690);


		if ( myhero->get_spell( spellslot::summoner1 )->get_spell_data( )->get_name_hash( ) == spell_hash( "SummonerFlash" ) )
            flash = plugin_sdk->register_spell( spellslot::summoner1, 400.f );
        else if ( myhero->get_spell( spellslot::summoner2 )->get_spell_data( )->get_name_hash( ) == spell_hash( "SummonerFlash" ) )
            flash = plugin_sdk->register_spell( spellslot::summoner2, 400.f );


	}

	void on_update()
	{
		if(myhero->is_dead())
		{
			return;
		}

		if(orbwalker->can_move(0.05f))
		{
			if(orbwalker->combo_mode())
			{
				if(q->is_ready() && combo::use_q->get_bool())
				{
					q_logic();
				}

				if (e->is_ready() && combo::use_e->get_bool())
				{
					e_logic();
				}

				if (r->is_ready() && combo::use_r->get_bool())
				{
					r_logic();
				}
				
			}
		}

		if(orbwalker->lane_clear_mode())
		{
			auto lane_minions = entity_list->get_enemy_minions();

			auto monsters = entity_list->get_jungle_mobs_minions();

			// minions

			lane_minions.erase(std::remove_if(lane_minions.begin(), lane_minions.end(), [] (game_object_script x)
			{
				return !x->is_valid_target(q->range()+300);
			}), lane_minions.end());

			// bixo da jg

			monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [] (game_object_script x)
			{
				return !x->is_valid_target(q->range()+300);
			}), monsters.end());monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [] (game_object_script x)
			{
				return !x->is_valid_target(q->range()+300);
			}), monsters.end());

			std::sort(monsters.begin(), monsters.end(), [] (game_object_script a, game_object_script b)
			{
				return a->get_max_health() > b->get_max_health();
			});

			if(!lane_minions.empty())
			{
				my_her_region = Position::Line;
			} else if (!monsters.empty())
			{
				my_hero_region = Position::Jungle;
			}

			if(!lane_minions.empty())
			{
				// usa o q pra mata os minion
				if(q->is_ready() && laneclear::use_q->get_bool())
				{
					// checa se meu boneco ta debaxo da torre dos inimigo
					if(myhero->is_under_enemy_turret())
					{
						// se a quantidade de inimigos embaixo da torre for == 0 pode usar o Q
						if(myhero->count_enemis_in_range(q->range()) == 0)
						{
							if(q->cast())
							{
								return;
							}
						}
					}
				}

				if(q->cooldown_time() > 0 && e->is_ready() && laneclear::use_e->get_bool())
				{
					// mesma logica de usar o q debaixo da torre pra ffarmar
					if ( myhero->is_under_enemy_turret( ) )
                        {
                            if ( myhero->count_enemies_in_range( e->range( ) ) == 0 )
                            {
                                if ( e->cast( ) )
                                {
                                    return;
                                }
                            }
				}

			}
		 

			if ( !monsters.empty( ) )
                {
                    // Logic responsible for monsters
                    if ( q->is_ready( ) && jungleclear::use_q->get_bool( ) )
                    {
                        if ( q->cast( ) )
                                return;                     
                    }

                    if ( q->cooldown_time( ) > 0 && e->is_ready( ) && jungleclear::use_e->get_bool( ))
                    {
                        if ( e->cast( ) )
                            return;
                    }
                }
			}
		}
	}
}

#pragma region q_logic
{
	void q_logic()
	{
		auto target = target_selector->get_target(500, damage_type::magical);

		if(target != nullptr)
		{
			if(target->get_distance(myhero)  <= q->range()) {
				if(q->cast())
				{
					return;
				} 
			}
		}
	}
}

#pragma region r_logic
{
	void r_logic()
	{
		if(target->get_distance(myhero) <= r->range() && target->is_visible_on_screen()) {
			if(r->cast)
			{
				return;
			}
		}
	}
}
