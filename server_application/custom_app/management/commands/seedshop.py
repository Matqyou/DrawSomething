from django.core.management.base import BaseCommand
from custom_app.models import ColorBundle
from custom_app.models import Color


class Command(BaseCommand):
    help = "Seed the shop with items"

    def handle(self, *args, **kwargs):
        bundles = [
            {
                "bundle_id": 1,
                "rarity": 1,
                "name": "Basic bundle",
                "price": 0,
            },
            {
                "bundle_id": 2,
                "rarity": 1,
                "name": "Basic 2 bundle",
                "price": 10,
            },
            {
                "bundle_id": 3,
                "rarity": 2,
                "name": "Red bundle",
                "price": 15,
            },
            {
                "bundle_id": 4,
                "rarity": 2,
                "name": "Green bundle",
                "price": 15,
            },
            {
                "bundle_id": 5,
                "rarity": 2,
                "name": "Blue bundle",
                "price": 15,
            },

            {
                "bundle_id": 6,
                "rarity": 3,
                "name": "Autumn bundle",
                "price": 20,
            },
            {
                "bundle_id": 7,
                "rarity": 3,
                "name": "Winter bundle",
                "price": 20,
            },
            {
                "bundle_id": 8,
                "rarity": 3,
                "name": "Spring bundle",
                "price": 20,
            },
            {
                "bundle_id": 9,
                "rarity": 3,
                "name": "Summer bundle",
                "price": 20,
            },
            {
                "bundle_id": 10,
                "rarity": 3,
                "name": "Ocean bundle",
                "price": 20,
            },
            {
                "bundle_id": 11,
                "rarity": 3,
                "name": "Forest bundle",
                "price": 20,
            },
            {
                "bundle_id": 12,
                "rarity": 3,
                "name": "Sunset bundle",
                "price": 20,
            },
            {
                "bundle_id": 13,
                "rarity": 3,
                "name": "Neon bundle",
                "price": 20,
            },
            {
                "bundle_id": 14,
                "rarity": 3,
                "name": "Pastel bundle",
                "price": 20,
            },
            {
                "bundle_id": 15,
                "rarity": 3,
                "name": "Gemstone bundle",
                "price": 20,
            },
            {
                "bundle_id": 16,
                "rarity": 3,
                "name": "Vintage bundle",
                "price": 20,
            },
        ]
        colors = [
            {
                "color_id": 1,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 2,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 3,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 4,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 5,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 6,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 28,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 255,
            },

            {
                "color_id": 27,
                "bundle_id": 2,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 128,
            },
            {
                "color_id": 22,
                "bundle_id": 2,
                "rarity": 1,
                "r": 0,
                "g": 128,
                "b": 255,
            },
            {
                "color_id": 23,
                "bundle_id": 2,
                "rarity": 1,
                "r": 128,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 24,
                "bundle_id": 2,
                "rarity": 1,
                "r": 255,
                "g": 128,
                "b": 0,
            },
            {
                "color_id": 25,
                "bundle_id": 2,
                "rarity": 1,
                "r": 128,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 26,
                "bundle_id": 2,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 128,
            },

            {
                "color_id": 7,
                "bundle_id": 3,
                "rarity": 1,
                "r": 64,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 8,
                "bundle_id": 3,
                "rarity": 1,
                "r": 128,
                "g": 0,
                "b": 0,

            },
            {
                "color_id": 9,
                "bundle_id": 3,
                "rarity": 1,
                "r": 192,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 10,
                "bundle_id": 3,
                "rarity": 1,
                "r": 255,
                "g": 64,
                "b": 64,
            },
            {
                "color_id": 11,
                "bundle_id": 3,
                "rarity": 1,
                "r": 255,
                "g": 128,
                "b": 128,
            },

            {
                "color_id": 12,
                "bundle_id": 4,
                "rarity": 1,

                "r": 0,
                "g": 64,
                "b": 0,
            },
            {
                "color_id": 13,
                "bundle_id": 4,
                "rarity": 1,
                "r": 0,
                "g": 128,
                "b": 0,
            },
            {
                "color_id": 14,
                "bundle_id": 4,
                "rarity": 1,
                "r": 0,
                "g": 192,
                "b": 0,
            },
            {
                "color_id": 15,
                "bundle_id": 4,
                "rarity": 1,
                "r": 64,
                "g": 255,
                "b": 64,
            },
            {
                "color_id": 16,
                "bundle_id": 4,
                "rarity": 1,
                "r": 128,
                "g": 255,
                "b": 128,
            },

            {
                "color_id": 17,
                "bundle_id": 5,
                "rarity": 1,

                "r": 0,
                "g": 0,
                "b": 64,
            },
            {
                "color_id": 18,
                "bundle_id": 5,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 128,
            },
            {
                "color_id": 19,
                "bundle_id": 5,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 192,
            },
            {
                "color_id": 20,
                "bundle_id": 5,
                "rarity": 1,
                "r": 64,
                "g": 64,
                "b": 255,
            },
            {
                "color_id": 21,
                "bundle_id": 5,
                "rarity": 1,
                "r": 128,
                "g": 128,
                "b": 255,
            },

            {
                "color_id": 29,
                "bundle_id": 6,
                "rarity": 2,
                "r": 158,
                "g": 77,
                "b": 23,
            },
            {
                "color_id": 30,
                "bundle_id": 6,
                "rarity": 1,
                "r": 255,
                "g": 114,
                "b": 0,
            },
            {
                "color_id": 31,
                "bundle_id": 6,
                "rarity": 1,
                "r": 255,
                "g": 202,
                "b": 89,
            },
            {
                "color_id": 32,
                "bundle_id": 6,
                "rarity": 1,
                "r": 73,
                "g": 193,
                "b": 99,
            },
            {
                "color_id": 33,
                "bundle_id": 6,
                "rarity": 1,
                "r": 131,
                "g": 224,
                "b": 100,
            },

            {
                "color_id": 34,
                "bundle_id": 7,
                "rarity": 3,
                "r": 70,
                "g": 130,
                "b": 180,
            },
            {
                "color_id": 35,
                "bundle_id": 7,
                "rarity": 2,
                "r": 135,
                "g": 206,
                "b": 235,
            },
            {
                "color_id": 36,
                "bundle_id": 7,
                "rarity": 2,
                "r": 176,
                "g": 196,
                "b": 222,
            },
            {
                "color_id": 37,
                "bundle_id": 7,
                "rarity": 1,
                "r": 240,
                "g": 248,
                "b": 255,
            },
            {
                "color_id": 38,
                "bundle_id": 7,
                "rarity": 1,
                "r": 230,
                "g": 230,
                "b": 250,
            },

            {
                "color_id": 39,
                "bundle_id": 8,
                "rarity": 3,
                "r": 250,
                "g": 172,
                "b": 190,
            },
            {
                "color_id": 40,
                "bundle_id": 8,
                "rarity": 2,
                "r": 255,
                "g": 192,
                "b": 203,
            },
            {
                "color_id": 41,
                "bundle_id": 8,
                "rarity": 2,
                "r": 255,
                "g": 239,
                "b": 213,
            },
            {
                "color_id": 42,
                "bundle_id": 8,
                "rarity": 1,
                "r": 144,
                "g": 238,
                "b": 144,

            },
            {
                "color_id": 43,
                "bundle_id": 8,
                "rarity": 1,

                "r": 152,
                "g": 251,
                "b": 152,
            },

            {
                "color_id": 44,
                "bundle_id": 9,
                "rarity": 3,
                "r": 255,
                "g": 69,
                "b": 0,
            },
            {
                "color_id": 45,
                "bundle_id": 9,
                "rarity": 2,
                "r": 255,
                "g": 140,
                "b": 0,
            },
            {
                "color_id": 46,
                "bundle_id": 9,
                "rarity": 2,
                "r": 255,
                "g": 215,
                "b": 0,
            },
            {
                "color_id": 47,
                "bundle_id": 9,
                "rarity": 1,
                "r": 255,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 48,
                "bundle_id": 9,
                "rarity": 1,
                "r": 255,
                "g": 165,
                "b": 79,
            },

            {
                "color_id": 49,
                "bundle_id": 10,
                "rarity": 3,
                "r": 25,
                "g": 25,
                "b": 112,
            },
            {
                "color_id": 50,
                "bundle_id": 10,
                "rarity": 2,
                "r": 0,
                "g": 100,
                "b": 148,
            },
            {
                "color_id": 51,
                "bundle_id": 10,
                "rarity": 2,
                "r": 0,
                "g": 119,
                "b": 190,
            },
            {
                "color_id": 52,
                "bundle_id": 10,
                "rarity": 1,
                "r": 0,
                "g": 191,
                "b": 243,
            },
            {
                "color_id": 53,
                "bundle_id": 10,
                "rarity": 1,
                "r": 72,
                "g": 209,
                "b": 204,
            },

            {
                "color_id": 54,
                "bundle_id": 11,
                "rarity": 3,
                "r": 34,
                "g": 139,
                "b": 34,
            },
            {
                "color_id": 55,
                "bundle_id": 11,
                "rarity": 2,
                "r": 85,
                "g": 107,
                "b": 47,
            },
            {
                "color_id": 56,
                "bundle_id": 11,
                "rarity": 2,
                "r": 107,
                "g": 142,
                "b": 35,
            },
            {
                "color_id": 57,
                "bundle_id": 11,
                "rarity": 1,
                "r": 124,
                "g": 252,
                "b": 0,
            },
            {
                "color_id": 58,
                "bundle_id": 11,
                "rarity": 1,
                "r": 173,
                "g": 255,
                "b": 47,
            },

            {
                "color_id": 59,
                "bundle_id": 12,
                "rarity": 3,
                "r": 220,
                "g": 20,
                "b": 60,
            },
            {
                "color_id": 60,
                "bundle_id": 12,
                "rarity": 2,
                "r": 255,
                "g": 69,
                "b": 0,
            },
            {
                "color_id": 61,
                "bundle_id": 12,
                "rarity": 2,
                "r": 255,
                "g": 140,
                "b": 0,
            },
            {
                "color_id": 62,
                "bundle_id": 12,
                "rarity": 1,
                "r": 255,
                "g": 215,
                "b": 0,
            },
            {
                "color_id": 63,
                "bundle_id": 12,
                "rarity": 1,
                "r": 255,
                "g": 160,
                "b": 122,
            },

            {
                "color_id": 64,
                "bundle_id": 13,
                "rarity": 3,
                "r": 237,
                "g": 39,
                "b": 184,
            },
            {
                "color_id": 65,
                "bundle_id": 13,
                "rarity": 2,
                "r": 57,
                "g": 234,
                "b": 145,
            },
            {
                "color_id": 66,
                "bundle_id": 13,
                "rarity": 2,
                "r": 191,
                "g": 87,
                "b": 245,
            },
            {
                "color_id": 67,
                "bundle_id": 13,
                "rarity": 1,
                "r": 89,
                "g": 234,
                "b": 245,
            },
            {
                "color_id": 68,
                "bundle_id": 13,
                "rarity": 1,
                "r": 234,
                "g": 245,
                "b": 67,
            },

            {
                "color_id": 69,
                "bundle_id": 14,
                "rarity": 3,
                "r": 255,
                "g": 182,
                "b": 193,
            },
            {
                "color_id": 70,
                "bundle_id": 14,
                "rarity": 2,
                "r": 221,
                "g": 160,
                "b": 221,
            },
            {
                "color_id": 71,
                "bundle_id": 14,
                "rarity": 2,
                "r": 173,
                "g": 216,
                "b": 230,
            },
            {
                "color_id": 72,
                "bundle_id": 14,
                "rarity": 1,
                "r": 255,
                "g": 228,
                "b": 181,
            },
            {
                "color_id": 73,
                "bundle_id": 14,
                "rarity": 1,
                "r": 152,
                "g": 251,
                "b": 152,
            },

            {
                "color_id": 74,
                "bundle_id": 15,
                "rarity": 3,
                "r": 75,
                "g": 0,
                "b": 130,
            },
            {
                "color_id": 75,
                "bundle_id": 15,
                "rarity": 2,
                "r": 220,
                "g": 20,
                "b": 60,
            },
            {
                "color_id": 76,
                "bundle_id": 15,
                "rarity": 2,
                "r": 0,
                "g": 100,
                "b": 0,
            },
            {
                "color_id": 77,
                "bundle_id": 15,
                "rarity": 1,
                "r": 255,
                "g": 215,
                "b": 0,
            },
            {
                "color_id": 78,
                "bundle_id": 15,
                "rarity": 1,
                "r": 192,
                "g": 192,
                "b": 192,
            },

            {
                "color_id": 79,
                "bundle_id": 16,
                "rarity": 3,
                "r": 139,
                "g": 69,
                "b": 19,
            },
            {
                "color_id": 80,
                "bundle_id": 16,
                "rarity": 2,
                "r": 160,
                "g": 82,
                "b": 45,
            },
            {
                "color_id": 81,
                "bundle_id": 16,
                "rarity": 2,
                "r": 205,
                "g": 133,
                "b": 63,
            },
            {
                "color_id": 82,
                "bundle_id": 16,
                "rarity": 1,
                "r": 222,
                "g": 184,
                "b": 135,
            },
            {
                "color_id": 83,
                "bundle_id": 16,
                "rarity": 1,
                "r": 245,
                "g": 245,
                "b": 220,
            }

        ]

        # ----- HANDLE BUNDLES -----
        existing_bundle_ids = set(ColorBundle.objects.values_list("bundle_id", flat=True))
        input_bundle_ids = set(bundle["bundle_id"] for bundle in bundles)

        # Delete removed bundles
        bundles_to_delete = existing_bundle_ids - input_bundle_ids
        ColorBundle.objects.filter(bundle_id__in=bundles_to_delete).delete()
        for bundle_id in bundles_to_delete:
            self.stdout.write(self.style.WARNING(f"Deleted bundle: #{bundle_id}"))

        # Upsert bundles
        for bundle in bundles:
            obj, created = ColorBundle.objects.get_or_create(bundle_id=bundle["bundle_id"], defaults=bundle)
            if not created:
                for key, value in bundle.items():
                    setattr(obj, key, value)
                obj.save()
                self.stdout.write(self.style.SUCCESS(f"Updated bundle: #{bundle['bundle_id']} {bundle['name']}"))
            else:
                self.stdout.write(self.style.SUCCESS(f"Created bundle: #{bundle['bundle_id']} {bundle['name']}"))

        # ----- HANDLE COLORS -----
        existing_color_ids = set(Color.objects.values_list("color_id", flat=True))
        input_color_ids = set(color["color_id"] for color in colors)

        # Delete removed colors
        colors_to_delete = existing_color_ids - input_color_ids
        Color.objects.filter(color_id__in=colors_to_delete).delete()
        for color_id in colors_to_delete:
            self.stdout.write(self.style.WARNING(f"Deleted color: #{color_id}"))

        # Upsert colors
        for color in colors:
            obj, created = Color.objects.get_or_create(color_id=color["color_id"], defaults=color)
            if not created:
                for key, value in color.items():
                    if key == "bundle_id":
                        obj.bundle_id = value
                    else:
                        setattr(obj, key, value)
                obj.save()
                self.stdout.write(self.style.SUCCESS(f"Updated color: #{color['color_id']}"))
            else:
                self.stdout.write(self.style.SUCCESS(f"Created color: #{color['color_id']}"))
